#pragma once

#include "../Defines.h"
#include "../NetHandSink.h"
#include "../include/TimerNode.h"

class ConnSucess;

class CGameSerSink : public CNetHandSink
{
public:
	CGameSerSink(CServices* pServices);
	~CGameSerSink();
public:
	virtual bool HandTimeMsg(uint16 uTimeID);
	virtual bool HandNetData(uint16, uint16, uint16, void*, DATASIZE);
	virtual void Connect();
	virtual void Close();
private:
	bool HandTestNetConn();
	bool HandMsgFromConnSrv(uint16, uint16, void*, DATASIZE);
private:
	void ConnectSucess(ConnSucess* pConn);
private:
	uint16 	m_nConnNo;
	uint8 	m_nTestNum;
	CTimer	m_timer_Link;
};