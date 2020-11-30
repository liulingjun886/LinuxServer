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
	void Init(const char* szIp);
	bool DisConnect();
	bool HandNetData(uint16, uint16, uint16, void*, uint32);
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
	uint16 		m_nTestNum;
	uint16 		m_nReConnectCount;
	
	uint16		m_nPeerSrvType;
	uint16 		m_nPeerSrvNo;
	
	CTimerNode 	m_timer_Link;
	CTimerNode 	m_timer_reconnect;
};

