#include <fstream>
#include "CConf.h"

using namespace std;
CConf::CConf(string szfile)
{
	parse(szfile);
}

CConf::~CConf()
{

}

void CConf::parse(string szfile)
{
	ifstream inf(szfile);
	if(inf.is_open() )
	{
		string line;
		string token = "=";

		while(getline(inf, line)) {

			if(line[0] == '#' || line[0] == '[' || line == "" ) continue;
			int pos = line.find_first_of(token, 0);
			if( pos < 0 ) break;

			string key = trim(line.substr(0, pos));
			string val = trim(line.substr(pos+1, line.length()));
			m_map.insert(pair<string, string>(key, val));
		}
	}
	inf.close();
}

string CConf::getString(const string key)
{
	auto iFind = m_map.find(key);
	if(iFind != m_map.end()) return iFind->second;
	return "";
}
int CConf::getINT(const string key)
{
	auto iFind = m_map.find(key);
	if(iFind != m_map.end())
	{
		try
		{
			return stoi(iFind->second);
		}
		catch(invalid_argument&)
		{
			return -1;
		}
	}
	return -1;
}
string CConf::trim_right(const string& src, const string& whitespaces)
{
	std::string str(src);
	return str.erase(str.find_last_not_of(whitespaces) + 1);

}
string CConf::trim_left(const string& src, const string& whitespaces)
{
	std::string str(src);
	return str.erase(0, str.find_first_not_of(whitespaces));

}
string CConf::trim(const string& src, const string& whitespaces)
{
	std::string str(src);
	return trim_left(trim_right(src, whitespaces), whitespaces);
}
