#ifdef WIN32

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <direct.h>
#include <Shlobj.h>
#pragma comment (lib, "Shell32.lib")  

#elif LINUX

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pwd.h>
#include <netdb.h>
#include <signal.h>
#include <cstring>

#endif

#include "System.h"

std::string System::IpAddress()
{
  static const std::string loopback("127.0.0.1");
  static std::string localip = loopback;
  if( localip != loopback )
    return localip;

  struct hostent *phe = NULL;
  struct in_addr addr;

#ifdef WIN32

  char ac[80];
  memset(ac, 0x00, 80);

  WSAData wsadata;
  WSAStartup(MAKEWORD(2,2), &wsadata);
  if(gethostname(ac, 80) == SOCKET_ERROR)
  {
    WSACleanup();
    return loopback;
  }

  phe = gethostbyname(ac);
  if(!phe)
  {
    WSACleanup();
    return loopback;
  }
  WSACleanup();
  std::memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
#elif LINUX

  struct utsname myname;
  if(uname(&myname) < 0 )
    return loopback;
  phe = gethostbyname(myname.nodename);
  if(!phe)
  {
    return loopback;
  }
  std::memcpy(&addr.s_addr, *(phe->h_addr_list), sizeof(addr.s_addr));
#endif
  localip = inet_ntoa(addr);
  return localip;
}

std::string System::ProcessID()
{
  static std::string str_pid("");
  if(str_pid != "") return str_pid;

#ifdef WIN32
  DWORD pid = GetCurrentProcessId();
  str_pid = std::to_string(pid);
#elif LINUX
  pid_t pid = getpid();
  str_pid = std::to_string(pid);
#endif
  return str_pid;
}


std::string System::CurrentDirectory()
{
  char buffer[256] = "";
#ifdef WIN32
#define getcwd _getcwd
#endif

  return (getcwd(buffer, sizeof(buffer))? std::string(buffer) : std::string(""));
}

std::string System::UserHome()
{
#ifdef WIN32
  WCHAR path[MAX_PATH + 1];
  if (SHGetSpecialFolderPathW(HWND_DESKTOP, path, CSIDL_DESKTOPDIRECTORY, FALSE))
  {
    std::wstring ws(path);
    std::string str(ws.begin(), ws.end());
    return str;
  }
#elif LINUX
  uid_t uid = getuid();
  struct passwd* pwd = getpwuid(uid);
  if(pwd != NULL)
    return pwd->pw_dir;
#endif
  return ".";
}

System::time System::Time()
{
  time systemtime = {0,};
#ifdef WIN32
  SYSTEMTIME t = {0,};
  ::GetLocalTime(&t);
  systemtime.year = t.wYear;
  systemtime.month = t.wMonth;
  systemtime.day = t.wDay;
  systemtime.hour = t.wHour;
  systemtime.minute = t.wMinute;
  systemtime.seconds = t.wSecond;
  systemtime.milliseconds = t.wMilliseconds;
#elif LINUX
  struct timeval val;
  struct tm *ptm;

  gettimeofday(&val, NULL);
  ptm = localtime(&val.tv_sec);
  systemtime.year = ptm->tm_year + 1900;
  systemtime.month = ptm->tm_mon+1;
  systemtime.day = ptm->tm_mday;
  systemtime.hour = ptm->tm_hour;
  systemtime.minute = ptm->tm_min;
  systemtime.seconds = ptm->tm_sec;
  systemtime.milliseconds = val.tv_usec/1000;
#endif
  return systemtime;
}

void System::SetDeamon()
{
#ifdef LINUX
  pid_t pid = fork();
  if( pid < 0)
    exit(1);
  else if ( pid != 0)
    exit(1);
  else if ( pid == 0)
  {
    signal(SIGHUP, SIG_IGN);
    close(0);
    close(1);
    close(2);

//    stdin = freopen("/dev/null", "r", stdin);
//    stdout = freopen("/dev/null", "w", stdout);
//    stderr = freopen("/dev/null", "w", stderr);
//
   setsid();
  }
#endif
}




