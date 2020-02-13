#include <string.h>
#include "Parser.h"
#include "Session.h"

Parser::Parser()
  : head_pos(0), body_pos(head_pos), payload_size(0), hasHeader(false)
{
  memset(buffer, 0x00, sizeof(MAX_BUFFER));
}

int Parser::Execute(Session* const s)
{
  int ret = Header(s);
  if( ret > 0 ) {
    ret = Payload(s);
    if( ret > 0 ) {
      s->Dispatch(buffer, payload_size);
    }
  }
  return ret;
}





