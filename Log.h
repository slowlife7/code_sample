#ifndef _LOG_H__
#define _LOG_H__

#include <string>
#include <functional>
#include <mutex>
//#include "types.h"

namespace LOG
{
  enum LEVEL
  {
    _NO_LOG_ = 0,
    _INFO_,
    _DETAIL_,
    _DEBUG_,
    _ERROR_ 
  };

  void Log_Info(const std::string& s);
  void Log_Detail(const std::string& s);
  void Log_Debug(const std::string& s);
  void Log_Error(const std::string& s);
#define Log(lv,s) LOG::Log_##lv("<"+_FUNC_+">"+s)

  class Logger
  {
    public:
      void  Level(const LOG::LEVEL lv);
      bool  Path(const std::string& path, const std::string& subdir);
      void  Name(const std::string& name);
      void  Name(std::function<std::string()> cbfunc);
      void  Prefix(std::function<std::string()> cbfunc);
      void  Print(const std::string& msg, const LOG::LEVEL level);

      static Logger& instance();

    private:
      static Logger*  ins;

      Logger();
      ~Logger();
      Logger(const Logger&);
      Logger& operator=(const Logger&);

      bool IsFile(const std::string& path);
      void PrintHelper(const std::string& file, const std::string& s, const LOG::LEVEL lv);

      friend class Cleanup;
      class Cleanup
      {
        public:
          ~Cleanup();
      };

      std::mutex mtx;

      LOG::LEVEL level;
      std::string name;
      std::string prefix;
      std::string path;
      std::string sub_dir;

      std::function<std::string()> cbName;
      std::function<std::string()> cbPrefix;
  };

  Logger& Ins();
};
#endif
