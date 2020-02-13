#include "ICommand.h"
#include <iostream>

ICommand::ICommand(byte_t op, byte_t fcode, byte_t scode, byte_t ver)
  
{
	/*
	.....

	*/
}

f_key ICommand::GetKeyValue()
{
	/*
	.....

	*/
  return key;
}

Stream_Ptr ICommand::GetResult()
{
  return stream;
}

Stream_Ptr ICommand::BindParameters()
{
  auto s = _BindParameters();

  /*
  .....

  */
  return params;
}

void ICommand::SetResult(Stream_Ptr& s)
{
  stream = s;
}


Stream_Ptr ICommand::_BindParameters()
{
	/*
	.....

	*/
  return s;
}
