#include "Log.h"
#include <fstream>

#ifdef WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
static const std::string slash = "\\";

#elif LINUX
#include <unistd.h>
#include <sys/stat.h>
#define _access(path,mode) access((path),(mode))
#define _mkdir(dir) mkdir((dir), 0755)
static const std::string slash = "/";
#endif

using namespace LOG;

void LOG::Log_Info(const std::string& s)
{
  Logger::instance().Print(s, _INFO_);
}
void LOG::Log_Detail(const std::string& s)
{
  Logger::instance().Print(s, _DETAIL_);
}
void LOG::Log_Debug(const std::string& s)
{
  Logger::instance().Print(s, _DEBUG_);
}
void LOG::Log_Error(const std::string& s)
{
  Logger::instance().Print(s, _ERROR_);
}

Logger& LOG::Ins()
{
  return Logger::instance();
}

Logger* Logger::ins = nullptr;

void Logger::Level(const LOG::LEVEL lv)
{
  std::lock_guard<std::mutex> lk(mtx);
  level = lv;
}

bool Logger::Path(const std::string& path, const std::string& sub_dir = "")
{
  std::lock_guard<std::mutex> lk(mtx);

  if(!IsFile(path + slash + sub_dir))
	  return false;
  
  this->path = std::move(path);
  this->sub_dir = std::move(sub_dir);

  return true;
}

void Logger::Name(const std::string& name)
{
  std::lock_guard<std::mutex> lk(mtx);
  this->name = std::move(name);
}

void Logger::Name(std::function<std::string()> cbfunc)
{
  std::lock_guard<std::mutex> lk(mtx);
  cbName = cbfunc;
}

void Logger::Prefix(std::function<std::string()> cbfunc)
{
  std::lock_guard<std::mutex> lk(mtx);
  cbPrefix = cbfunc;
}

void Logger::Print(const std::string& s, const LOG::LEVEL lv)
{
  
    std::lock_guard<std::mutex> lk(mtx);
    if(level < lv) return;
    if(cbName != nullptr) name = cbName();
    if(cbPrefix != nullptr) prefix = cbPrefix();
  

  std::string file = path+slash+sub_dir;
  if(!IsFile(file)) return;

  PrintHelper(file+slash+name, s, lv);
}

bool Logger::IsFile(const std::string& path)
{
  if(_access(path.c_str(), 0) < 0)
    return false;
  
  if(_access(path.c_str(), 6) < 0)
    return false;
  
  if(_mkdir(path.c_str()) < 0)
  {
    if(errno == EEXIST)
      return true;

    return false;
  }
  return true;
}

void Logger::PrintHelper(const std::string& file, const std::string& s, const LOG::LEVEL lv)
{
  std::ofstream out;
  out.open(file, std::ios_base::app);
  if(!out.good())
  {
    throw std::runtime_error("unable to initialize log file:"+file);
  }
  out<<prefix<<"["<<lv<<"]"<<s<<std::endl;
  out.close();
}

Logger& Logger::instance()
{
  static Cleanup cln;
  if(ins == nullptr)
    ins = new Logger();
  return *ins;
}

Logger::Logger()
  : level(LOG::_INFO_)
{}

Logger::~Logger()
{}

Logger::Cleanup::~Cleanup()
{
  delete Logger::ins;
  Logger::ins = nullptr;
}


