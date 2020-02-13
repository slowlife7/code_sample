#include "NXEvent.h"
#include "Stream.h"
#include "Log.h"
#include "cubeDef.h"
#include "cubePro.h"

struct stamp
{
  ushort_t year;
  byte_t month;
  byte_t day;
  byte_t hour;
  byte_t minute;
  byte_t second;
  ushort_t millisec;
  ushort_t mindiff;
  uint_t   utc;
};

std::shared_ptr<NXEvent> Generate(Stream_Ptr& s)
{
	/*
	.....

	*/
}

enum attachedtags
{
  tag_start = 0xaa,
  tag_cnid = 0xb0,
  tag_cnsq = 0xb1,
  tag_ctype = 0xb2,
  tag_uei = 0xb3,
  tag_ci = 0xb4,
  tag_queue = 0xb5,
  tag_waittime = 0xb6,
  tag_end = 0xff
};

static void supplement_decode(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}

void NXEvent::Parse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}


void CcseDown::SubParse(Stream_Ptr& s, cubeEventData * const data)
{
	data->event = cubeEK_CCSEDown;
}

void ServiceInit::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}

void Originated::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}

void NetworkReached::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}

void Delivered::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}


void Diverted::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}

void Queued::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}

void Established::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}
void ConnectionCleared::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}
void Held::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}
void Retrieved::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}
void Transferred::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}
void Conferenced::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}


void LoggedOn::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/

}

void LoggedOff::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}

void NotReady::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}

void Ready::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}

void OtherWork ::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}

void ACW ::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}

void PwdConfirm::SubParse(Stream_Ptr& s, cubeEventData* const data)
{
	/*
	.....

	*/
}



