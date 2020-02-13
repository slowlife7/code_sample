#ifndef __NXAPP_H__
#define __NXAPP_H__

#include <string>
#include <queue>
#include "types.h"
#include "IListener.h"

using namespace type;
class Stream;
class Session;
class NXApp : public IListener
{
    std::vector<NXSession_Ptr> sessions;
    uint_t session_cnt;

    std::queue<Stream_Ptr> evtque;
    std::weak_ptr<NXSession> act_session;
	std::vector<Command_Ptr> retrycommands;

    virtual void OnEvent(Stream_Ptr& s);
    virtual void OnClosed(Session* s);
    virtual void OnResponse(Stream_Ptr& s);
    virtual void OnOpened(Session* s);

    bool canretry;
    void ActChange();
    void ForceActChange();

  public:
    NXApp();

    int Add(const std::string ip, const ushort_t port);
    int Open(const byte_t type, const byte_t link, const std::string dn, const std::string appname);
    void Disconnect();
    int AddMonitor();
    int RemoveMonitor();

	void AddRetryCommand(Command_Ptr& cmd);
	void RemoveRegister(Command_Ptr& cmd);

	int Send(Command_Ptr& cmd);
	int ExecuteAllSessions(Command_Ptr& cmd);
    int Execute(Command_Ptr& cmd, const uint_t timeout);
    Stream_Ptr GetResult(Command_Ptr& cmd);
    Stream_Ptr Notify();
};

#endif
