#pragma once
#include "NetHandSink.h"
#include "DataServer.h"

class CServices;

class CDataCliSink : public CNetHandSink
{
public:
	CDataCliSink(CServices* m_pNetSer);
	~CDataCliSink();
public:
	virtual bool HandTimeMsg(USHORT nTimeID);
	virtual bool HandNetData(USHORT, USHORT, USHORT, void*, UINT);
private:
	bool HandMainMsgCenter(USHORT,	  USHORT, void*, UINT);
private:
	CDataServer* m_pServer;
	CTimer m_timer_Link;
	USHORT m_nTestNum;
};


