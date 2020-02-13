#ifndef __NXAPPASYNC_H__
#define __NXAPPASYNC_H__


#include <string>
#include <queue>
#include <functional>
#include <memory>
#include "types.h"
#include "IListener.h"

using namespace type;

class NXAppAsync : public IListener
{
  std::weak_ptr<NXSession> act_session;
  std::vector<NXSession_Ptr> sessions;
  uint_t session_cnt;

  virtual void OnEvent(Stream_Ptr& s);
  virtual void OnClosed(Stream_Ptr& s);
  virtual void OnResponse(Stream_Ptr& s);

  bool canretry;
  
  std::function<void(Stream_Ptr&)> OnRecvComplete;
  std::function<void(Stream_Ptr&)> OnEvtComplete;

  public:
  NXAppAsync();

  int Add(const std::string ip, const ushort_t port);
  int Open(const byte_t type, const byte_t link, const std::string dn, const std::string appname);
  void Close();
  int AddMonitor();

  int Send(Command_Ptr& cmd);
  void Recv(std::function<void(Stream_Ptr&)> cb);
  void Event(std::function<void(Stream_Ptr&)> cb);
};

#endif