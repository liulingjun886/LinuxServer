#pragma once
#include "../NetHandSink.h"
#include "../include/TimerNode.h"
#include "../PacketParse.h"

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
	bool HandNetData(uint16, uint16, CInputPacket& inPacket);
	bool HandTimeMsg(uint16 uTimeID);
private:
	bool HandMainMsgRoom(uint16, CInputPacket& inPacket);
	bool HandMainMsgNet(uint16, CInputPacket& inPacket);

	bool HandMsgFromCenterSrv(uint16, CInputPacket& inPacket);
	bool HandMsgFromUserSrv(uint16, CInputPacket& inPacket);
	bool HandMsgFromDataSrv(uint16, CInputPacket& inPacket);
private:
	bool HandMsgTestConn();
	void ConnectSucess(CInputPacket& inPacket);
	void RegGameSrv();
private:
	uint16 		m_nTestNum;
	uint16 		m_nReConnectCount;
	
	uint16		m_nPeerSrvType;
	uint16 		m_nPeerSrvNo;
	
	CTimerNode 	m_timer_Link;
	CTimerNode 	m_timer_reconnect;
};

