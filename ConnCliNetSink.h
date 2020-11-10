#pragma once
#include "CliNetSink.h"
#include "TimerNode.h"

class CServices;

class CConnCliNetSink : public CCliNetSink
{
	USHORT m_nTestLink;
	char m_nReConnectCount;
	CTimerNode m_timerConnTest;
	CTimerNode m_timerReConn;
public:
	CConnCliNetSink(CServices* pNet);
	~CConnCliNetSink();
public:
	void Init(UINT nIp);
	bool DisConnect();
	bool HandNetData(USHORT,USHORT, USHORT, void*, USHORT);
	bool HandTimeMsg(USHORT uTimeID);
private:
	bool HandMainMsgNet(USHORT nIndex, USHORT nSub, void* pData, USHORT nDataSize);
};

