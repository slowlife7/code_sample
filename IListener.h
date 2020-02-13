#ifndef _ILISTENER_H__
#define _ILISTENER_H__

#include "types.h"
class Session;
class IListener
{
  public:
    virtual ~IListener() {}
    virtual void OnResponse(Stream_Ptr& s) = 0;
    virtual void OnEvent(Stream_Ptr& s) = 0;
    virtual void OnClosed(Session* s) = 0;
    virtual void OnOpened(Session* s) = 0;
};

#endif


