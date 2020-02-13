#ifndef __TYPES_H__
#define __TYPES_H__

#include <memory>
#include <vector>
#include <string>

namespace type
{
  typedef unsigned char   byte_t;
  typedef unsigned short  ushort_t;
  typedef unsigned int    uint_t;
  typedef int       int_t;
  typedef short     short_t;

  typedef struct dnstring
  {
    byte_t      length;
    byte_t      type;
    std::string dn;
  }dnstring;

  typedef struct ustring
  {
    ushort_t length;
    std::string data;

    ustring()
    {
    }
    ustring(const char* str)
      : data(str)
    {
      length = data.length();
    };
  }ustring;

  typedef enum {
    async = 0,
    sync = 1
  }open_type;

#define ERR_MAP(V) \
  V( -5, NO_CONN, "no exist connection") \
  V( -4, AL_CONN, "already exist connection") \
  V( -3, NO_BUFF, "not enough buffer") \
  V( -2, INV_OP, "invalid opcode") \
  V( -1, LOSS_CONN, "lost connnection") \
  V(  1, OK, "success") \
  V(3007, INV_GATE, "wrong gate number") \
  V(3010, NO_EVENT, "there is no event at the queue") \
  V(3013, TIME_OUT, "can't recieve the packet in specific time") \

#define ERR_GEN(val, name, s) NX_##name = val,
  typedef enum {
    ERR_MAP(ERR_GEN)
    MAX_ERRORS
  }err_code_t;
#undef ERR_GEN

#define STRERR_GEN(val, name, s) case NX_##name : return s;
static const char* nx_strerr(err_code_t err) {
  switch(err) {
    ERR_MAP(STRERR_GEN)
    default:
      return "unknown error";
  }
}
#undef STRERR_GEN

#ifndef WIN32
#define _FUNC_ std::string(__func__)
#else
#define _FUNC_ std::string(__FUNCTION__)
#endif
};

class Stream;
typedef std::shared_ptr<Stream> Stream_Ptr;

class ICommand;
typedef std::shared_ptr<ICommand> Command_Ptr;
typedef std::vector<std::string> result_values;

class Session;
class NXSession;
typedef std::shared_ptr<Session> Session_Ptr;
typedef std::shared_ptr<NXSession> NXSession_Ptr;
typedef type::ushort_t f_invoke;
typedef type::ushort_t   f_key;

#endif
