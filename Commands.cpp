#include "Commands.h"
#include <exception>

CmdOpenServer::CmdOpenServer(byte_t type, byte_t link, std::string dn, std::string appname, 
    ushort_t inv)
: ICommand(0xa1, 0x45, 0x46, 0x28)
{}

Stream_Ptr CmdOpenServer::_BindParameters()
{
  Stream_Ptr s = std::make_shared<Stream>();
  /*
  .....

  */
  return s;
}

CmdCloseServer::CmdCloseServer()
  : ICommand(0xa1, 0x45, 0x1e, 0x28)
{

}

/*
.....

*/