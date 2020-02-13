#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <string>

namespace System
{
  struct time
  {
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int seconds;
    unsigned int milliseconds;
  };

  std::string  IpAddress();
  std::string  ProcessID();
  std::string  CurrentDirectory();
  std::string  UserHome();
  void         SetDeamon();
  time     Time();

};

#endif
