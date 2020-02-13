#include <iostream>
#include "NXApp.h"
#include "NXSession.h"
#include "NXParser.h"
#include "Commands.h"
#include "Stream.h"
#include "System.h"
#include "Log.h"

using namespace LOG;
NXApp::NXApp()
  : session_cnt(0), canretry(true)
{
}

void NXApp::OnResponse(Stream_Ptr& s)
{

}

void NXApp::OnOpened(Session* s)
{
  ++session_cnt;
  Log(Detail, "session_cnt["+std::to_string(session_cnt)+"]\n");
}

void NXApp::AddRetryCommand(Command_Ptr& cmd)
{
	retrycommands.push_back(cmd);
}
void NXApp::RemoveRegister(Command_Ptr& cmd)
{
	
}

void NXApp::OnEvent(Stream_Ptr& s)
{
	/*
	.....

	*/
}
void NXApp::ActChange()
{
	/*
	.....

	*/
}


Stream_Ptr NXApp::Notify()
{
	/*
	.....

	*/
}

void NXApp::OnClosed(Session* s)
{
	/*
	.....

	*/
}

void NXApp::ForceActChange()
{
	/*
	.....

	*/
}

int NXApp::Add(const std::string ip, const ushort_t port)
{
	/*
	.....

	*/
  return 1;
}

int NXApp::AddMonitor()
{
	/*
	.....

	*/
  return act_ret;
}

int NXApp::RemoveMonitor()
{
	/*
	.....

	*/
  return act_ret;
}

int NXApp::Open(const byte_t type, const byte_t link, const std::string dn, const std::string appname)
{
	/*
	.....

	*/
  return (act_ret > 0)? act_ret : NX_TIME_OUT;
}

void NXApp::Disconnect()
{
  canretry = false;
  for(auto s : sessions)
  {
    Log(Info, "send invoke");
    s->Disconnect();
  }
}

int NXApp::ExecuteAllSessions(Command_Ptr& cmd)
{
	/*
	.....

	*/
  return act_ret;
}
int NXApp::Send(Command_Ptr& cmd)
{
  Log(Info, "send-invoke");
  auto act = act_session.lock();
  if(!act) return NX_LOSS_CONN;
  return act->Send(cmd);
}
int NXApp::Execute(Command_Ptr& cmd, const uint_t timeout)
{
  Log(Info, "command-invoke");
  auto act = act_session.lock();
  if(!act) return NX_LOSS_CONN;
  return act->Execute(cmd, timeout);
}

Stream_Ptr NXApp::GetResult(Command_Ptr& cmd)
{
  Log(Info, "result-invoke\n");
  auto act = act_session.lock();
  if(!act) return nullptr;
  return act->GetResult(cmd);
}
