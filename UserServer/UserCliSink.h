#pragma once
#include "UserServer.h"
#include "../include/TimeNode.h"

class CUserCliSink : public CNetHandSink
{
public:
	CUserCliSink();
	~CUserCliSink();
public:
	virtual bool HandTimeMsg(USHORT nTimeID);
	virtual bool HandNetData(USHORT, USHORT, USHORT, void*, UINT);
private:
	bool HandMainMsgCenter(USHORT,    USHORT, void*, UINT);
private:
	CUserServer* m_pServer;
	CTimer m_timer_Link;
	USHORT m_nTestNum;
};


