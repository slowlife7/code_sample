#ifndef __ICOMMAND_H__
#define __ICOMMAND_H__

#include "types.h"
#include "Stream.h"

class ICommand
{
  public:
    ICommand(byte_t op, byte_t fcode, byte_t scode, byte_t ver);
    virtual ~ICommand() {}

    Stream_Ptr BindParameters();
    Stream_Ptr GetResult();
    void SetResult(Stream_Ptr& s);

    f_key GetKeyValue();

    void SetGate(const uint_t gate) { this->gate = gate; }
    void SetInv(const ushort_t inv) { this->inv = inv; }
    virtual Stream_Ptr    _BindParameters(); 

  private:
    Stream_Ptr stream;

	/*
	.....

	*/
};

#endif
