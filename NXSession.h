#ifndef __NXSESSION_H__
#define __NXSESSION_H__

#include <map>
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <thread>
#include <atomic>

#include "types.h"
#include "Session.h"

class Parser;
class NXSession : public Session
{
    virtual void OnDispatch(Stream_Ptr& s);
    virtual void _Response(Stream_Ptr& s);
    virtual bool _Event(Stream_Ptr& s);
    virtual void _Close();

    void GetPayload(Stream_Ptr& s);
    std::map<f_key, Command_Ptr> command;

    uint_t gate;
    ushort_t inv;
    Command_Ptr opsvr;

    enum class HA_Status
    {
      unknown = 0x00,
      active,
      standby
    };

    enum class Mode
    {
      single = 0x01,
      HA
    };

    HA_Status ha_status;
    Mode      mode;

    std::thread retrythread;
    std::atomic<bool> isretry;
    
  public:
    NXSession(std::string ip, unsigned short port);
    virtual ~NXSession() {}

    bool IsActive() const
    {
      return (ha_status == HA_Status::active)? true : false;
    }
    void ToActive()
    {
      ha_status = HA_Status::active;
    }

    int Open(Command_Ptr& opnsvr);
    virtual int Open();
    virtual int ReOpen(std::vector<Command_Ptr> v);

    void HA(const bool flag);
    void Disconnect();

    void RunReconnect();
    void ExitReconnect();
	
    int AddMonitor();
    int RemoveMonitor();

    int Send(Command_Ptr& cmd);
    int Execute(Command_Ptr& cmd, const uint_t timeout = 3);
    Stream_Ptr GetResult(Command_Ptr& cmd);

};

#endif
