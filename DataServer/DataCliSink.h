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
	virtual bool HandNetData(uint16, uint16, uint16, void*, uint32);
private:
	bool HandMainMsgFromCenterSrv(uint16,	  uint16, void*, uint32);
private:
	CTimer m_timer_Link;
	uint16 m_nTestNum;
};


