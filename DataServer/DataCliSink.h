#pragma once
#include "../NetHandSink.h"
#include "DataServer.h"
#include "../include/TimerNode.h"

class CServices;

class CDataCliSink : public CNetHandSink
{
public:
	CDataCliSink(CServices* m_pNetSer);
	~CDataCliSink();
public:
	virtual bool HandTimeMsg(uint16 nTimeID);
	virtual bool HandNetData(uint16, uint16, uint16, void*, DATASIZE);
	virtual bool DisConnect();
private:
	void SendMsgToCenterSrv(uint16, uint16, void*, DATASIZE);
	bool HandMainMsgFromCenterSrv(uint16,	  uint16, void*, DATASIZE);
private:
	CTimer m_timer_Link;
	uint16 m_nTestNum;

	uint16 m_nReConnectCount;
	CTimer m_timer_reconnect;
};


