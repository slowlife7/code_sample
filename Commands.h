#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "ICommand.h"

using namespace type;

class CmdOpenServer : public ICommand
{
  public:
    CmdOpenServer(byte_t type, byte_t link, std::string dn, std::string appname, ushort_t inv);

    virtual ~CmdOpenServer() { }
    virtual Stream_Ptr _BindParameters();

  private:
	  /*
	  .....

	  */
};

class CmdCloseServer : public ICommand
{
  public:
    CmdCloseServer();

    virtual ~CmdCloseServer() { }
};

/*
.....

*/

class CmdMonitorStart : public ICommand
{
  public:
    CmdMonitorStart();

    virtual ~CmdMonitorStart() {}
};

class CmdMonitorStop : public ICommand
{
  public:
    CmdMonitorStop();
    virtual ~CmdMonitorStop() {}
};

class CmdAddDevice : public ICommand
{
  public:
    CmdAddDevice(const char* device, const byte_t type, const byte_t link);

    virtual ~CmdAddDevice() { }
    virtual Stream_Ptr _BindParameters();

  private:
    byte_t type;
    byte_t link;
    std::string device;
};

class CmdPwdConfirm : public ICommand
{
  public:
    CmdPwdConfirm(const uint_t device, const uint_t confirm, const char* attach);

    virtual ~CmdPwdConfirm() { }
    virtual Stream_Ptr _BindParameters();

  private:
    uint_t device;
    uint_t confirm;
    type::ustring attach;
};

class CmdWaitTime : public ICommand
{
  public:
    CmdWaitTime(const uint_t tenant, const char* queue);

    virtual ~CmdWaitTime() {}
    virtual Stream_Ptr _BindParameters();

  private:
    uint_t tenant;
    std::string queue;
};

class CmdQueryAgentStatusEx : public ICommand
{
public:
	CmdQueryAgentStatusEx(const uint_t device, const std::string agentid);

	virtual ~CmdQueryAgentStatusEx() { }
	virtual Stream_Ptr _BindParameters();

private:
	uint_t device;
	std::string agentid;
};

class CmdAgentReadyGet : public ICommand
{
public:
	CmdAgentReadyGet(const ushort_t group, const ushort_t part, const ushort_t maxcnt);

	virtual ~CmdAgentReadyGet() { }
	virtual Stream_Ptr _BindParameters();

private:
	byte_t center;
	ushort_t maxcnt;
	ushort_t group;
	ushort_t part;
};

class CmdSetAgentStatus : public ICommand
{
public:
	CmdSetAgentStatus(const std::string device, const byte_t agent_mode, const std::string agentid,
		const std::string group, const std::string part, const byte_t reasoncode, const byte_t workmode);

	virtual ~CmdSetAgentStatus() { }
	virtual Stream_Ptr _BindParameters();

private:
	byte_t agent_mode;
	byte_t work_mode;
	byte_t reasoncode;
	std::string device;
	std::string agentid;
	std::string group;
	std::string part;
};

class CmdDial : public ICommand
{
  public:
    CmdDial(const char* calling, const char* called,
        const char* uui, const char* accode, const char* authcode, const char* uei, const char* ci, const byte_t normal);

    virtual Stream_Ptr _BindParameters();
    virtual ~CmdDial() {}

  private:
    std::string calling;
    std::string called;
    std::string uui;
    std::string accode;
    std::string authcode; 
    type::ustring uei;
    type::ustring ci;
    byte_t    normal;
};

class CmdSingleStepTransfer : public ICommand
{
public:
	CmdSingleStepTransfer(const char* calling, const uint_t callid, const char* called, const byte_t mode, 
		const char* uui, const char* accccode, const char* authcode, const char* uei, const char* ci);
	virtual Stream_Ptr _BindParameters();
    virtual ~CmdSingleStepTransfer() {}
private:
	byte_t mode;
	uint_t callid;
	std::string calling;
	std::string called;
	std::string uui;
	std::string acccode;
	std::string authcode;
	type::ustring uei;
	type::ustring ci;
};

class CmdAnswer : public ICommand
{
  public:
    CmdAnswer(const char* calling, const uint_t callid);

    virtual Stream_Ptr _BindParameters();
    virtual ~CmdAnswer() {}

  private:
    uint_t callid;
    std::string calling;
};

class CmdClear : public ICommand
{
  public:
    CmdClear(const char* calling, const uint_t callid);

    virtual Stream_Ptr _BindParameters();
    virtual ~CmdClear() { }

  private:
    byte_t      type;
    ushort_t    empty;
    uint_t      callid;
    std::string calling;
};

class CmdHold : public ICommand
{
  public:
    CmdHold(const char* calling, const uint_t callid);

    virtual Stream_Ptr _BindParameters();
    virtual ~CmdHold() { }

  private:
    byte_t      type;
    uint_t      callid;
    std::string calling;
};

class CmdRetrieve : public ICommand
{
  public:
    CmdRetrieve(const char* calling, const uint_t callid);

    virtual Stream_Ptr _BindParameters();
    virtual ~CmdRetrieve() { }

  private:
    byte_t      type;
    ushort_t    empty;
    uint_t      callid;
    std::string calling;
};

class CmdConsult : public ICommand
{
  public:
    CmdConsult(const char* calling, const uint_t callid, const char* called,
        const char* uui, const char* acccode, const char* authcode,
        const char* uei, const char* ci);

    virtual Stream_Ptr _BindParameters();
    virtual ~CmdConsult() { }

  private:
    byte_t type;
    uint_t callid;
    std::string calling;
    std::string called;
    std::string uui;
    std::string accode;
    std::string authcode;
    ustring uei;
    ustring ci;
};

class CmdTransfer : public ICommand
{
  public:
    CmdTransfer(const char* held_calling, const uint_t held_callid,
        const char* act_calling, const uint_t act_callid);

    virtual Stream_Ptr _BindParameters();
    virtual ~CmdTransfer() { }

  private:
    byte_t act_type;
    byte_t held_type;
    uint_t act_callid;
    uint_t held_callid;
    std::string act_calling;
    std::string held_calling;
};
class CmdConference : public ICommand
{
  public:
    CmdConference(const char* held_calling, const uint_t held_callid,
        const char* act_calling, const uint_t act_callid);

    virtual Stream_Ptr _BindParameters();
    virtual ~CmdConference() { }

  private:
    byte_t act_type;
    byte_t held_type;
    uint_t act_callid;
    uint_t held_callid;
    std::string act_calling;
    std::string held_calling;
};

class CmdSetUEIData : public ICommand
{
public:
	CmdSetUEIData(const byte_t serverid, const uint_t callid, const char* uei);

	virtual Stream_Ptr _BindParameters();
	virtual ~CmdSetUEIData() {}

private:
	byte_t serverid;
	uint_t callid;
	std::string uei;
};

class CmdPutTraceInfo : public ICommand
{
public:
	CmdPutTraceInfo(const uint_t cnid, const uint_t callid, const char* channel, const char* service_code,
		const char* service_code_name, const char* digit, const char* dnis, const char* ani, 
		const byte_t reason, const char* target, const char* attach);

	virtual Stream_Ptr _BindParameters();
	virtual ~CmdPutTraceInfo() {}

private:
	uint_t cnid;
	uint_t callid;
	std::string channel;
	std::string service_code;
	std::string service_code_name;
	std::string digit;
	std::string dnis;
	std::string ani;
	byte_t reason;
	std::string target;
	type::ustring attach;
};

class CmdIVRRegister : public ICommand
{
public:
	CmdIVRRegister(const uint_t serverid, const uint_t callid, const char* dn, const char* ani, const char* uei,
		const uint_t isallow);

	virtual Stream_Ptr _BindParameters();
	virtual ~CmdIVRRegister() {}

private:
	uint_t serverid;
	uint_t callid;
	std::string dn;
	std::string ani;
	type::ustring uei;
	uint_t isallow;
};

class CmdIVRRelease : public ICommand
{
public:
	CmdIVRRelease(const uint_t serverid, const uint_t callid, const char* dn, const char* target);

	virtual Stream_Ptr _BindParameters();
	virtual ~CmdIVRRelease() {}

private:
	uint_t serverid;
	uint_t callid;
	std::string dn;
	std::string target;
};

class CmdIVRAllowRouting : public ICommand
{
public:
	CmdIVRAllowRouting(const uint_t serverid, const uint_t callid, const char* dn, const uint_t isallow);

	virtual Stream_Ptr _BindParameters();
	virtual ~CmdIVRAllowRouting() {}

private:
	uint_t serverid;
	uint_t callid;
	std::string dn;
	uint_t isallow;
};

#endif
