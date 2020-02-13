#include "Parser.h"
#include "Socket.h"
#include "Session.h"
#include "IListener.h"
#include "Log.h"

#include <sstream>
#include <string>


static std::string toHex(byte_t* bytes, const int size)
{
  char szmsg[10] = "";
  std::stringstream ss;
  for(int i = 0; i<size; i++)
  {
    sprintf(szmsg, "%02x ", bytes[i]);
    ss << szmsg;
  }
  return ss.str();
}

Session::Session(std::string ip, unsigned short port)
    : ip(ip), port(port), socket(new Socket()),
  terminatedWorkerThread(false),  status(Session_Status::CLOSED)
{

}

Session::~Session()
{
  Close();
  delete socket;
}

void Session::SetParser(std::shared_ptr<Parser> parser)
{
  this->parser =parser;
}

int Session::Open()
{
  if(status == Session_Status::OPENED)
    return 1;

  int ret = socket->Connect(ip, port);
  if( ret != 1) return ret;

  terminatedWorkerThread = false;
  std::packaged_task<int()> wk([&]()->int {

      status = Session_Status::OPENED;
      
      while(!terminatedWorkerThread)
      {
        int ret = socket->Select();
        if( ret < 0)  terminatedWorkerThread = true;
        else if(ret == 0) {
        continue;
        }
        ret = parser->Execute(this);
        if( ret == NX_LOSS_CONN)
          terminatedWorkerThread = true;
      }
	  if(status != Session_Status::CLOSING)
	  {
		status = Session_Status::CLOSED;
		OnClose(this);
	  }
      return 1;
      });
  worker = wk.get_future(); 
  std::thread(std::move(wk)).detach(); 
  return ret;
}

int Session::ReOpen()
{
  return 1;
}

int Session::Close()
{
  status = Session_Status::CLOSING;
  terminatedWorkerThread = true;
  socket->Close();
  //worker.get();
  return 1;
}
int Session::Send(Stream& s)
{
	 auto hex = toHex(s.Buffer(), s.Size());
  Log(Info, hex);
  int ret = socket->Send((char*)s.Buffer(), s.Size());
  if( ret < 0 ) {
    Close();
    return ret;
  }
  return ret;
}
int Session::Send( Stream& s, const unsigned int timeout)
{
  auto hex = toHex(s.Buffer(), s.Size());
  Log(Info, hex);
  int ret = socket->Send((char*)s.Buffer(), s.Size());
  if( ret < 0 ) {
    Close();
    return ret;
  }

  std::unique_lock<std::mutex> lk(mtx);
  if(cv.wait_for(lk, std::chrono::seconds(timeout)) == std::cv_status::timeout)
    return 3013;

  return 1;
}

int Session::Recv(char* buffer, const size_t payload_size)
{
  return socket->Recv(buffer, payload_size);
}

bool Session::IsAlive() const
{
  return (status == Session_Status::OPENED)? true : false;
}

void Session::AddListener(IListener* listener)
{
  this->listener = listener;
}

void Session::OnResponse(Stream_Ptr& s)
{
  if(listener != nullptr)
  {
    {
      std::unique_lock<std::mutex> lk(mtx);
      _Response(s);
      cv.notify_one();
    }
    listener->OnResponse(s);
  }
}

void Session::OnEvent(Stream_Ptr& s)
{
  if(listener != nullptr)
  {
    if(_Event(s))
      listener->OnEvent(s);
  }
}

void Session::OnClose(Session* s)
{
  if(listener != nullptr)
  {
    _Close();
    listener->OnClosed(s);
  }
}

void Session::OnOpen(Session* s)
{
  if(listener != nullptr)
  {
    listener->OnOpened(s);
  }
}

int Session::Dispatch(char* buffer, const size_t payload_size)
{
  auto hex = toHex((byte_t*)buffer, payload_size+3);
  Log(Info, hex);
  auto s = std::make_shared<Stream>((byte_t*)buffer, (ushort_t)payload_size+3);
  OnDispatch(s);
  return 1;
}
