#include "NXSession.h"
#include "NXParser.h"

using namespace type;
int NXParser::Header(Session* const s)
{
	/*
	.....

	*/
  return NX_OK;

}

int NXParser::Payload(Session* const s)
{
  int ret = s->Recv(buffer + head_pos + body_pos, payload_size - body_pos);
  /*
  .....

  */
}


