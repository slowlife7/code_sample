#include <iostream>
#include "NXAdapter.h"
#include "NXApp.h"
#include "ICommand.h"
#include "System.h"
#include "Log.h"
#include "NXEvent.h"
#include "NXConfigReader.h"
#include "cubeDef.h"
#include "cubePro.h"


#define NX_VERSION_MAJOR 5
#define NX_VERSION_MINOR 2
#define NX_VERSION_RELEASE 0
#define NX_VERSION_PATCH 0

#define NX_VERSION ((NX_VERSION_MAJOR << 24) | \
                    (NX_VERSION_MINOR << 16) | \
                    (NX_VERSION_RELEASE << 8) | \
                    (NX_VERSION_PATCH))

#define NX_STRINGIFY(v) NX_STRINGIFY_HELPER(v)
#define NX_STRINGIFY_HELPER(v) #v

#define NX_VERSION_STRING NX_STRINGIFY(NX_VERSION_MAJOR) "." \
                               NX_STRINGIFY(NX_VERSION_MINOR) "." \
                               NX_STRINGIFY(NX_VERSION_RELEASE) "." \
                               NX_STRINGIFY(NX_VERSION_PATCH)
using namespace LOG;
uint_t NXAdapter::version()
{
	return NX_VERSION;
}
const char* NXAdapter::version_string()
{
	return NX_VERSION_STRING;
}

void NXAdapter::AddGate(NXApp_Ptr& app)
{
  std::lock_guard<std::mutex> lk(mtx);
  apps[++gate] = app;
}

void NXAdapter::DelGate(const uint_t gate)
{
  std::lock_guard<std::mutex> lk(mtx);
  apps.erase(gate);
}

NXApp_Ptr NXAdapter::FindGate(const uint_t gate)
{
  std::lock_guard<std::mutex> lk(mtx);
  auto f = apps.find(gate);
  if ( f == apps.end()) {
    Log(Error, "No Exist Gate["+std::to_string(gate)+"]");
    return nullptr;
  }
  return f->second;
}

NXAdapter* NXAdapter::ins = nullptr;
NXAdapter& NXAdapter::instance()
{
  static Cleanup up;
  if(ins == nullptr) 
    ins = new NXAdapter();
  return *ins;
}

void NXAdapter::LogLevel(const uint_t lv)
{
	NXPROPERTIES.set_log_level(lv);
	Logger::instance().Level((LOG::LEVEL)(lv));
}
void NXAdapter::LogPath(const char* const path)
{
	NXPROPERTIES.set_log_path(path);
	Logger::instance().Path(path, "nxcapilog");
}

NXAdapter::NXAdapter()
	: gate(0)
{
  Logger::instance().Level(LOG::_ERROR_);
  std::string _userhome = System::UserHome();
  int ret = Logger::instance().Path(_userhome, "nxcapilog");

  Logger::instance().Name([]()->std::string {
        System::time t = System::Time();
        char date[40] = "";
        std::sprintf(date, "%04d%02d%02d%02d", t.year, t.month, t.day, t.hour);
        return "NXCAPI_"+std::string(date)+"_"+System::IpAddress()+"["+System::ProcessID()+"].log";
      });

  Logger::instance().Prefix([]()->std::string {
        char buf[40] = "";
        System::time t = System::Time();
        std::sprintf(buf, "[%02d:%02d:%02d:%03d]", t.hour, t.minute, t.seconds, t.milliseconds);
        return buf;
      });

  NXPROPERTIES.set_io_context_seconds(0);
  NXPROPERTIES.set_io_context_useconds(3000);
  NXPROPERTIES.set_heart_beat_time(40000);
  NXPROPERTIES.set_dial_timeout(3);
  NXPROPERTIES.set_answer_timeout(2);
  NXPROPERTIES.set_sst_timeout(2);
  NXPROPERTIES.set_log_level(5);
  NXPROPERTIES.set_log_path(System::UserHome().c_str());
  NXPROPERTIESLOAD

  Log(Info, "io_context_seconds["+std::to_string(NXPROPERTIES.io_context_seconds())+"]");
  Log(Info, "io_context_useconds["+std::to_string(NXPROPERTIES.io_context_useconds())+"]");
  Log(Info, "heart_beat_time["+std::to_string(NXPROPERTIES.hearbeat_time())+"]");
  Log(Info, "log_level["+std::to_string(NXPROPERTIES.log_level())+"]");
  Log(Info, "log_path["+NXPROPERTIES.log_path()+"]");
}

NXAdapter::~NXAdapter()
{

}

NXAdapter::Cleanup::~Cleanup()
{
  delete NXAdapter::ins;
  NXAdapter::ins = nullptr;
}

int NXAdapter::Open(uint_t* gate, const byte_t type, const byte_t link, const std::string& ip, const ushort_t port, const std::string& device, const std::string& appname)
{
  Log(Info, "NXAdapter Version["+version_string()+"]");
  NXApp_Ptr ap = std::make_shared<NXApp>();
  ap->Add(ip, port);

  int ret = ap->Open(type, link, device, appname);
  if( ret > 0 )
    AddGate(ap);
  *gate = this->gate;
  return ret;
}


void NXAdapter::Close(const uint_t gate)
{
  auto f = FindGate(gate);
  if( f ) 
  {
    f->Disconnect();
    DelGate(gate);
  }
}


int NXAdapter::ExecuteAllSessions(const uint_t gate, Command_Ptr& cmd)
{
	auto f = FindGate(gate);
  if ( f )
	  return f->ExecuteAllSessions(cmd);
  return NX_INV_GATE;
}

int NXAdapter::Send(const uint_t gate, Command_Ptr& cmd)
{
	auto f = FindGate(gate);
	if( f )
		return f->Send(cmd);
	return NX_INV_GATE;
}

int NXAdapter::Execute(const uint_t gate, Command_Ptr& cmd, const uint_t timeout)
{
  auto f = FindGate(gate);
  if ( f )
    return f->Execute(cmd, timeout);
  return NX_INV_GATE;
}
Stream_Ptr NXAdapter::Result(const uint_t gate, Command_Ptr& cmd)
{
  auto f = FindGate(gate);
  if( f )
    return f->GetResult(cmd);
  return nullptr;
}
Stream_Ptr NXAdapter::Notify(const uint_t gate)
{
  auto f = FindGate(gate);
  if ( f )
    return f->Notify();
  return nullptr;
}

int NXAdapter::Event(const uint_t gate, cubeEventData* data)
{
  auto f = FindGate(gate);
  if (!f)
  {
    data->event = (cubeEventKind_Def)cubeEK_CCSEDown;
    return NX_OK;
  }

  auto stream = f->Notify();
  if(!stream)
    return NX_NO_EVENT;
  
  auto g = Generate(stream);
  if(g)
  {
    g->Parse(stream,data);
	return NX_OK;
  }
  return NX_NO_EVENT;
}

/*
.....

*/
