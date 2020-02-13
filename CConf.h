#ifndef __CCONF_H__
#define __CCONF_H__

#include <string>
#include <map>
#include <algorithm>

class CConf
{
  private:
    std::map<std::string, std::string> m_map;
    void parse(std::string szfile);

    std::string trim_right(const std::string& src, const std::string& whitespaces=" \t\r\n");
    std::string trim_left(const std::string& src, const std::string& whitespaces=" \t\r\n");
    std::string trim(const std::string& src, const std::string& whitespaces=" \t\r\n");

  public:
    CConf(std::string szfile);
    ~CConf();

    std::string getString(const std::string key);
    int getINT(const std::string key);
};

#endif