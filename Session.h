#ifndef _SESSION_H__
#define _SESSION_H__

#include <atomic>
#include <mutex>
#include <thread>
#include <future>
#include <memory>
#include <functional>
#include <condition_variable>

#include "types.h"
#include "Stream.h"

class Parser;
class Socket;
class IListener;
class Stream;
enum class Session_Status
{
  CLOSED = 0x01,
  CLOSING,
  OPENED = 0x10
};

class Session
{
  public:
    Session(std::string ip, unsigned short port);
    virtual ~Session();
    virtual void  OnDispatch(Stream_Ptr& s) = 0;
    virtual void _Response(Stream_Ptr& s) = 0;
    virtual bool _Event(Stream_Ptr& s) = 0;
    virtual void _Close() = 0;

    bool IsAlive() const;
    virtual int Open();
    virtual int ReOpen();
    int Close();

	int Send(Stream& s);
    int Send(Stream& s, const unsigned int timeout);
    void AddListener(IListener* listener);
    void SetParser(std::shared_ptr<Parser> parser);

    void OnResponse(Stream_Ptr& s);
    void OnEvent(Stream_Ptr& s);
    void OnClose(Session* s);
	void OnOpen(Session* s);

    int Recv(char* buffer, const size_t payload_size);
    int Dispatch(char* buffer, const size_t payload_size);

  private:
    std::shared_ptr<Parser> parser;
    std::atomic<bool> terminatedWorkerThread;
    std::future<int> worker;

    Session_Status status;

    Socket* socket;
    IListener* listener;

    std::mutex mtx;
    std::condition_variable cv;

    std::string ip;
    unsigned short port;
};

#endif
