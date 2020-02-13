#ifndef _PARSER_H__
#define _PARSER_H__

class Session;
class Parser
{
  public:
    Parser();
    virtual ~Parser() {}
    int Execute(Session* const s);

  private:
    virtual int Header(Session* const s) = 0;
    virtual int Payload(Session* const s) = 0;

  protected:
    int head_pos;
    int body_pos;
    int payload_size;
    bool hasHeader;

    enum {
      MAX_BUFFER = 4096
    };
    char buffer[4096];
};

#endif
