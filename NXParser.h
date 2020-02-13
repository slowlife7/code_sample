#include "Parser.h"

class NXParser : public Parser
{
  public:
  NXParser() : Parser() {} 
  virtual ~NXParser() {}
  private:
  virtual int Header(Session* const s);
  virtual int Payload(Session* const s);

};
