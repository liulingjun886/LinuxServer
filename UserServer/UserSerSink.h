#pragma once
#include "UserServer.h"
#include "../include/core/TimerNode.h"
#include "../NetHandSink.h"
#include "../PacketParse.h"

class CUserSerSink : public CNetHandSink
{
public:
	CUserSerSink(CServices* pServices);
	~CUserSerSink();
public:
	virtual bool HandTimeMsg(uint16 uTimeID);
	virtual bool HandNetData(uint16, uint16, CInputPacket& inPacket);
	virtual void Connect();
private:
	bool HandTestNetConn();
	bool HandMainMSgGameSer(uint16, CInputPacket& inPacket);
	bool HandMainMsgConnSer(uint16, CInputPacket& inPacket);
private:
	CTimer m_timer_Link;
	uint16 m_nTestNum;
};