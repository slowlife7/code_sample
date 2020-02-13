#include "NXSession.h"
#include "NXParser.h"
#include "Commands.h"
#include "Log.h"

  NXSession::NXSession(std::string ip, unsigned short port)
: Session(ip,port), ha_status(HA_Status::unknown), isretry(true), gate(0), inv(1)
{

}

void NXSession::_Response(Stream_Ptr& s)
{
  GetPayload(s);
}

bool NXSession::_Event(Stream_Ptr& s)
{
	/*
	.....

	*/
  return false;
}

void NXSession::_Close()
{
  ha_status = HA_Status::unknown;
}

void NXSession::GetPayload(Stream_Ptr& s)
{
	/*
	.....

	*/
}

void NXSession::HA(const bool flag)
{
	/*
	.....

	*/
}

int NXSession::Open()
{
  int ret = Session::Open();
  if( ret == 1)
  {
	  /*
	  .....

	  */
    return ret;
  }
  return ret;
}

int NXSession::Open(Command_Ptr& opnsvr)
{
  int ret = Session::Open();
  if( ret == 1)
  {
    this->opsvr = opnsvr;
    ret = Execute(opnsvr);
    if( ret == 1)
    {
      auto stream = GetResult(opnsvr);

		/*
			.....
		  
		 */
    }
    return ret;
  }
  return ret;
}
int NXSession::ReOpen(std::vector<Command_Ptr> v)
{
  std::thread worker = std::thread([&]() {

	  std::queue<Command_Ptr> retrycommand;

	  for(auto c : v)
		retrycommand.push(c);
	  
      int open_ret = 0;
      int monitor_ret = 0;
      isretry = true;
      while ( isretry)
      {
		  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
		  open_ret = Open();
		  Log(Detail, "open_result["+std::to_string(open_ret)+"]");
		  if(open_ret != 1)
		  {
			  std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
			  std::chrono::milliseconds mil = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
			  std::this_thread::sleep_for(std::chrono::milliseconds(3000)-mil);
			  continue;
		  }

		  /*
		  .....

		  */

		  std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
		  std::chrono::milliseconds mil = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
		  std::this_thread::sleep_for(std::chrono::milliseconds(3000)-mil);
      }

  });

  worker.detach();
  retrythread = std::move(worker);
  return 1;
}

void NXSession::Disconnect()
{
	/*
	.....

	*/
  Session::Close();
}

int NXSession::AddMonitor()
{
  Command_Ptr cmd = std::make_shared<CmdMonitorStart>();
  Log(Info,"send-invoke["+std::to_string(inv)+"]");
  int ret = Execute(cmd);
  ushort_t invoke = 0;
  if( ret == 1)
  {
	  /*
	  .....

	  */
  }
  Log(Info, "result-invoke["+std::to_string(ret)+"]["+std::to_string(invoke)+"]\n");
  return ret;
}

int NXSession::RemoveMonitor()
{
  Command_Ptr cmd = std::make_shared<CmdMonitorStop>();
  Log(Info,"send-invoke["+std::to_string(inv)+"]");
  int ret = Execute(cmd);
  ushort_t invoke = 0;
  if( ret > 0)
  {
	  /*
	  .....

	  */
  }
  Log(Info, "result-invoke["+std::to_string(ret)+"]["+std::to_string(invoke)+"]\n");
  return ret;
}

int NXSession::Send(Command_Ptr& cmd)
{
	/*
	.....

	*/
  auto s = cmd->BindParameters();
  return Session::Send(*s);
}

int NXSession::Execute(Command_Ptr& cmd, const uint_t timeout)
{
  f_key key = cmd->GetKeyValue();

  /*
  .....

  */

  command.insert(std::pair<f_key, Command_Ptr>(key, cmd));
  auto s = cmd->BindParameters();

  return Session::Send(*s, timeout);
}

Stream_Ptr NXSession::GetResult(Command_Ptr& cmd)
{
  f_key key = cmd->GetKeyValue();
  auto f = command.find(key);
  if ( f != command.end() )
  {
    Stream_Ptr ret = std::move(f->second->GetResult());
    command.erase(f);
    return ret;
  }
  return nullptr;
}

void NXSession::OnDispatch(Stream_Ptr& s)
{
	/*
	.....

	*/
}
void NXSession::ExitReconnect()
{
  isretry = false;
  if(retrythread.joinable())
    retrythread.join();
}
