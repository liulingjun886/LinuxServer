#pragma once
#include "UserServer.h"
#include "../include/core/TimerNode.h"
#include "../Defines.h"
#include "../NetHandSink.h"
#include "../PacketParse.h"

class CUserCliSink : public CNetHandSink
{
public:
	CUserCliSink(CServices* pServices);
	~CUserCliSink();
public:
	virtual bool HandTimeMsg(uint16 nTimeID);
	virtual bool HandNetData(uint16, uint16, CInputPacket& inPacket);
	virtual bool DisConnect();
private:
	bool HandMainMsgFromCenter(    uint16, CInputPacket& inPacket);
private:
	CTimer m_timer_Link;
	uint16 m_nTestNum;

	uint16 m_nReConnectCount;
	CTimer m_timer_reconnect;
};


