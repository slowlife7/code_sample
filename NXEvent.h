#ifndef __EVENT_H__
#define __EVENT_H__

#include <memory>
#include "types.h"

class NXEvent;
struct cubeEventData;

std::shared_ptr<NXEvent> Generate(Stream_Ptr& s);

class NXEvent
{
  public:
    void Parse(Stream_Ptr& s, cubeEventData * const data);

  private:
    virtual void SubParse(Stream_Ptr& s, cubeEventData * const data) = 0;
};

class CcseDown : public NXEvent
{
	virtual void SubParse(Stream_Ptr& s, cubeEventData * const data);
};

class ServiceInit : public NXEvent
{
    virtual void SubParse(Stream_Ptr& s, cubeEventData * const data);
};

class Originated : public NXEvent
{
    virtual void SubParse(Stream_Ptr& s, cubeEventData * const data);
};

class NetworkReached : public NXEvent
{
	 virtual void SubParse(Stream_Ptr& s, cubeEventData * const data);
};

class Delivered : public NXEvent
{
    virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class Diverted : public NXEvent
{
	virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class Queued : public NXEvent
{
	virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class Established : public NXEvent
{
  virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class ConnectionCleared : public NXEvent
{
  virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class Held : public NXEvent
{
  virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class Retrieved : public NXEvent
{
  virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class Transferred : public NXEvent
{
  virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class Conferenced : public NXEvent
{
  virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class LoggedOn : public NXEvent
{
	virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class LoggedOff : public NXEvent
{
	virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class NotReady : public NXEvent
{
	virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class Ready : public NXEvent
{
	virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class OtherWork : public NXEvent
{
	virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class ACW : public NXEvent
{
	virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};

class PwdConfirm : public NXEvent
{
	virtual void SubParse(Stream_Ptr& s, cubeEventData* const data);
};


#endif
