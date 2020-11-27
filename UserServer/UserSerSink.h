#pragma once
#include "UserServer.h"
#include "../include/TimerNode.h"
#include "../NetHandSink.h"

class CUserSerSink : public CNetHandSink
{
public:
	CUserSerSink(CServices* pServices);
	~CUserSerSink();
public:
	virtual bool HandTimeMsg(uint16 uTimeID);
	virtual bool HandNetData(uint16, uint16, uint16, void*, uint32);
	virtual void Connect();
private:
	bool HandMainMSgGameSer(uint16, uint16, void*, uint32);
	bool HandMainMsgConnSer(uint16, uint16, void*, uint32);
	bool HandTestNetConn();
private:
	CTimer m_timer_TestLink;
	uint16 m_nNum;
};