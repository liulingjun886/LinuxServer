#pragma once
#include "../NetHandSink.h"
#include "DataServer.h"
#include "../include/TimerNode.h"
#include "../PacketParse.h"

class CServices;

class CDataCliSink : public CNetHandSink
{
public:
	CDataCliSink(CServices* m_pNetSer);
	~CDataCliSink();
public:
	virtual bool HandTimeMsg(uint16 nTimeID);
	virtual bool HandNetData(uint16, uint16, CInputPacket& inPacket);
	virtual bool DisConnect();
private:
	void SendMsgToCenterSrv(COutputPacket& out);
	bool HandMainMsgFromCenterSrv(	  uint16, CInputPacket& inPacket);
	void RegDataSrv();
private:
	CTimer m_timer_Link;
	uint16 m_nTestNum;

	uint16 m_nReConnectCount;
	CTimer m_timer_reconnect;
};


