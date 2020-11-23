#pragma once
#include "UserServer.h"
#include "../include/TimeNode.h"

class CUserSerSink : public CNetHandSink
{
public:
	CUserSerSink(CServices* pServices);
	~CUserSerSink();
public:
	virtual bool HandTimeMsg(USHORT uTimeID);
	virtual bool HandNetData(USHORT, USHORT, USHORT, void*, UINT);
	virtual void Connect();
private:
	bool HandMainMSgGameSer(USHORT, USHORT, void*, UINT);
	bool HandMainMsgConnSer(USHORT, USHORT, void*, UINT);
	bool HandTestNetConn();
private:
	CTimer m_timer_TestLink;
	USHORT m_nNum;
};