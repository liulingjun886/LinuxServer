#pragma once
#include "UserServer.h"
#include "../include/TimerNode.h"
#include "../include/types.h"
#include "../NetHandSink.h"

class CUserCliSink : public CNetHandSink
{
public:
	CUserCliSink(CServices* pServices);
	~CUserCliSink();
public:
	virtual bool HandTimeMsg(USHORT nTimeID);
	virtual bool HandNetData(USHORT, USHORT, USHORT, void*, UINT);
private:
	bool HandMainMsgCenter(USHORT,    USHORT, void*, UINT);
private:
	CTimer m_timer_Link;
	USHORT m_nTestNum;
};


