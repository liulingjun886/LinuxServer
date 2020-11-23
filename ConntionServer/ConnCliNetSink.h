#pragma once
#include "CliNetSink.h"
#include "TimerNode.h"

class CServices;

class CConnCliNetSink : public CNetHandSink
{
public:
	CConnCliNetSink(CServices* pNet);
	~CConnCliNetSink();
public:
	void Init(UINT nIp);
	bool DisConnect();
	bool HandNetData(USHORT,USHORT, USHORT, void*, USHORT);
	bool HandTimeMsg(USHORT uTimeID);
private:	
	bool HandMsgFromCenterSrv(USHORT, USHORT, void*, USHORT);
	bool HandMsgFromGameSrv(USHORT, USHORT, void*, USHORT);
	bool HandMsgFromUserSrv(USHORT, USHORT, void*, USHORT);

	void SendData(USHORT, USHORT, USHORT, void*, USHORT);
	void RegConnSrv();
private:
	USHORT m_nPeerSerType;
	USHORT m_nPeerSerNo;
	USHORT m_nTestLink;
	char m_nReConnectCount;
	CTimerNode m_timerConnTest;
	CTimerNode m_timerReConn;
};

