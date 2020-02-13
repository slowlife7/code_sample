#ifdef WIN32 

#define close closesocket
//#define errno WSAGetLastError()
//#define EAGAIN WSAEWOULDBLOCK
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#elif LINUX
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#define INVALID_SOCKET -1
typedef struct timeval TIMEVAL;
#endif

#define INVALID_SOCKET_ERROR -1
#define SUCCESS 1

#include "Socket.h"

Socket::Socket()
{
  select_tv.tv_sec = 0;
  select_tv.tv_usec = 1000;

  status_tv.tv_sec = 2;
  status_tv.tv_usec = 0;

  hb.Init(300000);

#ifndef WIN32
  signal(SIGPIPE, SIG_IGN);
#endif
}
#include <iostream>

int Socket::Connect(const std::string& ip, const unsigned short port)
{
#ifdef WIN32
  WSADATA wsaData;
  WORD version = MAKEWORD(2, 2);
  if(WSAStartup(version, &wsaData) < 0)
    return INVALID_SOCKET;
#endif

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if( fd < 0)
    return INVALID_SOCKET;
 
  if(nonblockmode) SetNonBlock();

  struct sockaddr_in svr;
  svr.sin_family = AF_INET;
  svr.sin_addr.s_addr = inet_addr(ip.c_str());
  svr.sin_port = htons(port);

  int ret = connect(fd, (struct sockaddr*)&svr, sizeof(svr));
  if ( ret == 0 )
    return SUCCESS;
  else if( ret < 0)
  {
	if ( ConnectionStatus() >= 0)
		return SUCCESS;
  }

  return INVALID_SOCKET;
}

void Socket::SetNonBlock()
{
#ifdef WIN32
  unsigned long mode = 1;
  ioctlsocket(fd, FIONBIO, &mode);
#elif LINUX
  int mode = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, mode | O_NONBLOCK);
#endif
}

int Socket::ConnectionStatus()
{
  fd_set wset;
  TIMEVAL tv;

  FD_ZERO(&wset);
  FD_SET(fd, &wset);

  tv.tv_sec = status_tv.tv_sec;
  tv.tv_usec = status_tv.tv_usec;

#ifdef WIN32
  int ret = select(fd, NULL, &wset, NULL, &tv);
#elif LINUX
  int ret = select(fd+1, NULL, &wset, NULL, &tv);
#endif
  if( ( ret > 0 ) && FD_ISSET(fd, &wset) )
  {
#ifdef WIN32
    return SUCCESS;
#elif LINUX
    int error = 0;
    socklen_t len = sizeof(error);
    if(getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
    {
      close(fd);
      return INVALID_SOCKET;
    }
    
    if(error != 0)
    {
      close(fd);
      return INVALID_SOCKET;
    }
#endif
    return SUCCESS;
  }
  close(fd);
  return INVALID_SOCKET;
}

void Socket::ConnectionStatusTimeOut(const _TIMEVAL& tv)
{
  status_tv = tv;
}
void Socket::SelectTimeOut(const _TIMEVAL& tv)
{
  select_tv = tv;
}

void Socket::HeartBeatTime(const unsigned int milliseconds)
{
  hb.Init(milliseconds);
}
void Socket::NonBlockMode(bool mode)
{
  nonblockmode = mode;
}

int Socket::Select()
{
  fd_set rset;
  FD_ZERO(&rset);
  FD_SET(fd, &rset);

  TIMEVAL tv;
  tv.tv_sec = select_tv.tv_sec;
  tv.tv_usec = select_tv.tv_usec;

  int ret = select(fd + 1, &rset, NULL, NULL, &tv);
  if( ret < 0) return INVALID_SOCKET_ERROR;
  else if( ret > 0) {
    if(FD_ISSET(fd, &rset)) {
      hb.Update();
    }
  }
  if(!hb.IsTimeOut()) {
    Close();
    return INVALID_SOCKET_ERROR;
  } 
  return ret;
}

void Socket::Close()
{
  struct linger ling;
  ling.l_linger = 0;
  ling.l_onoff = 1;
  setsockopt(fd, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling));
  close(fd);
  //fd = -1;
}

int Socket::Send(char* buffer, const size_t len)
{
  return io_wrap(send, buffer, len);
}

int Socket::Recv(char* buffer, const size_t len)
{
  return io_wrap(recv, buffer, len);
}

int Socket::io_wrap(io_func f, char* buffer, int len)
{
  int offset = 0;
  int remainedLen = len;
  while(remainedLen > 0 )
  {
#ifdef WIN32
    int ret = f(fd, buffer+offset, len - offset, 0);
#elif LINUX
    int ret = f(fd, buffer+offset, len - offset);
#endif

  //  std::cout<<"s_ret:"<<ret<<std::endl;
    //if(!hb.IsTimeOut()) goto invalid;
    if( ret == 0 || !hb.IsTimeOut()  )
      goto invalid;
    else if ( ret == INVALID_SOCKET_ERROR)
    {
      int error = errno;
      if( error == EAGAIN ) goto end;
      goto invalid;
    }
    offset += ret;
    remainedLen -= ret;
  }

end:
  hb.Update();
  return offset;

invalid:
  Close();
  return INVALID_SOCKET_ERROR;
}

void Socket::HeartBeat::Init(const unsigned int milliseconds)
{
  heartbeattime = milliseconds;
  start = end =  std::chrono::system_clock::now();
}
void Socket::HeartBeat::Update()
{
  start = std::chrono::system_clock::now();
}
bool Socket::HeartBeat::IsTimeOut()
{
  end = std::chrono::system_clock::now();
  std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  if(duration.count() >= heartbeattime) return false;
  return true;
}

