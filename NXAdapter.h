#ifndef __NXADAPTER_H__
#define __NXADAPTER_H__


#include <memory>
#include <string>
#include <mutex>
#include <unordered_map>
#include "types.h"
#include "Stream.h"

#define NXADAPTER    NXAdapter::instance()
#define SENDTOCUBE(gate, cmd, rtn) 							\
	do {													\
		int ret = NXAdapter::instance().Execute(gate, cmd); \
		if( ret == cubeSuccess) {							\
			ushort_t inv = 0;								\
			rtn = NXAdapter::instance().Result(gate, cmd); 	\
			if(!rtn) return ret;							\
		}													\
	} while(0);												\


class NXApp;
typedef std::shared_ptr<NXApp> NXApp_Ptr;
typedef std::weak_ptr<NXApp> NXApp_Weak;
struct cubeEventData;
using namespace type;
class NXAdapter
{
  uint_t gate;
  std::unordered_map<uint_t, NXApp_Ptr> apps;
  std::mutex mtx;


  uint_t	  version();
  const char* version_string();

  //mutex
  void AddGate(NXApp_Ptr& app);
  void DelGate(const uint_t gate);
  NXApp_Ptr FindGate(const uint_t gate);

  NXAdapter();
  ~NXAdapter();
  NXAdapter(const NXAdapter&);
  NXAdapter& operator=(const NXAdapter&);

  class Cleanup
  {
    public:
      ~Cleanup();
  };
  friend class Cleanup;

  static NXAdapter* ins;

  public:
    static NXAdapter& instance();

	void LogLevel(const uint_t lv);
	void LogPath(const char* const path);

    int Open(uint_t* gate, const byte_t type, const byte_t link, const std::string& ip, const ushort_t port, const std::string& device, const std::string& appname);
  
    void Close(const uint_t gate);
   
	/*
	.....

	*/

	int ExecuteAllSessions(const uint_t gate, Command_Ptr& cmd);
    int Execute(const uint_t gate, Command_Ptr& cmd, const uint_t timeout = 3);
	int Send(const uint_t gate, Command_Ptr& cmd);

    Stream_Ptr Result(const uint_t gate, Command_Ptr& cmd);
    Stream_Ptr Notify(const uint_t gate);

    int Event(const uint_t gate, cubeEventData* data);
};



#endif
