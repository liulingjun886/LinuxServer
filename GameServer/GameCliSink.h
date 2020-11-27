#pragma once
#include "../NetHandSink.h"
#include "../include/TimerNode.h"

class CServices;
class ConnSucess;

class CGameCliSink : public CNetHandSink
{
public:
	CGameCliSink(CServices* pNet);
	~CGameCliSink();
public:
	void Init(uint32 nIp);
	bool DisConnect();
	bool HandNetData(uint16,uint16, uint16, void*, uint16);
	bool HandTimeMsg(uint16 uTimeID);
private:
	bool HandMainMsgRoom(uint16,uint16, void*, uint16);
	bool HandMainMsgNet(uint16, uint16, void*, uint16);

	bool HandMsgFromCenterSrv(uint16, uint16, void*, uint16);
	bool HandMsgFromUserSrv(uint16, uint16, void*, uint16);
	bool HandMsgFromDataSrv(uint16, uint16, void*, uint16);
private:
	bool HandMsgTestConn();
	void ConnectSucess(ConnSucess* pConn);
	void RegGameSrv();
private:
	char 		m_nTestLink;
	char 		m_nReConnectCount;
	uint16		m_nPeerSrvType;
	uint16 		m_nPeerSrvNo;
	CTimerNode 	m_timerConnTest;
	CTimerNode 	m_timerReConn;
};

