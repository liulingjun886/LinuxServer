#pragma once
#include "NetConnSerManager.h"
#include "CliNetSink.h"
#include "TimerNode.h"


class CServices;

class CGameCliNetSink : public CNetHandSink
{
public:
	CGameCliNetSink(CServices* pNet);
	~CGameCliNetSink();
public:
	void Init(UINT nIp);
	bool DisConnect();
	bool HandNetData(USHORT,USHORT, USHORT, void*, USHORT);
	bool HandTimeMsg(USHORT uTimeID);
private:
	bool HandMainMsgRoom(USHORT,USHORT, void*, USHORT);
	bool HandMainMsgNet(USHORT, USHORT, void*, USHORT);

	bool HandMsgFromCenterSrv(USHORT, USHORT, USHORT, void*, USHORT);
	bool HandMsgFromUserSrv(USHORT, USHORT, USHORT, void*, USHORT);
	bool HandMsgFromDataSrv(USHORT, USHORT, USHORT, void*, USHORT);
private:
	USHORT 		m_nTestLink;
	char 		m_nReConnectCount;
	CTimerNode 	m_timerConnTest;
	CTimerNode 	m_timerReConn;
	bool (CGameCliNetSink::*m_pHandFun)(USHORT, USHORT, void*, USHORT);
};

