#pragma once
#include "../NetHandSink.h"
#include "../MemDataBaseEnginer/MemDataBaseEnger.h"
#include "DataServer.h"
#include "../include/core/TimerNode.h"
#include "../PacketParse.h"

class CServices;

class CDataSerSink : public CNetHandSink
{
public:
	CDataSerSink(CServices* m_pNetSer);
	~CDataSerSink();
public:
	void Connect();
	bool HandNetData(uint16, uint16, CInputPacket& inPacket);
	bool HandTimeMsg(uint16 uTimeId);
private:
	bool TestNetLink();
	bool HandMainMsgFromGameSer(uint16,CInputPacket& inPacket);
private:
	bool HandTestNetConn();
private:
	CTimer m_timer_Link;
	uint16 m_nTestNum;
};