
#ifndef _SOCKET_H__
#define _SOCKET_H__

#include <string>
#include <chrono>
#include <functional>

#ifdef WIN32
#include <WinSock2.h>
#define fd_s SOCKET
  typedef std::function<int(fd_s, char*, int, int)> io_func;
#elif LINUX
  typedef int fd_s;
  typedef std::function<int(fd_s, char*, int)> io_func;
#define recv read
#define send write
#endif
  typedef struct __TIMEVAL
  {
    unsigned int tv_usec;
    unsigned int tv_sec;
  }_TIMEVAL;

  class Socket
  {
    public:
      Socket();
      void ConnectionStatusTimeOut(const _TIMEVAL& tv);
      void SelectTimeOut(const _TIMEVAL& tv);
      void HeartBeatTime(const unsigned int milliseconds);
      void NonBlockMode(bool mode);
      int  Select();

      int Connect(const std::string& ip, const unsigned short port);
      void Close();

      int  Send(char* buffer, const size_t len);
      int  Recv(char* buffer, const size_t len);

    private:
      fd_s fd;
      _TIMEVAL select_tv;
      _TIMEVAL status_tv;
      bool nonblockmode;

      class HeartBeat
      {
        public:
          void Init(const unsigned int milliseconds);
          void Update();
          bool IsTimeOut();
        private:
          std::chrono::system_clock::time_point start;
          std::chrono::system_clock::time_point end;
          unsigned int heartbeattime;
      };

      HeartBeat hb;
      int io_wrap(io_func func, char* buffer, int len);

      void SetNonBlock();
      int  ConnectionStatus();
      Socket(const Socket&);
      Socket& operator=(const Socket&);
  };

#endif
