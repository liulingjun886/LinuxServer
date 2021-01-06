#pragma once
#include "../Defines.h"
#include "../NetHandSink.h"
#include "../include/TimerNode.h"
#include "../PacketParse.h"

class CServices;
class ConnSucess;

class CConnCliSink : public CNetHandSink
{
public:
	CConnCliSink(CServices* pNet);
	~CConnCliSink();
public:
	void Init(const char* szIp);
	bool DisConnect();
	void Close();
	bool HandNetData(uint16, uint16, CInputPacket& inPacket);
	bool HandTimeMsg(uint16 uTimeID);
private:	
	bool HandMsgFromCenterSrv(uint16, CInputPacket& inPacket);
	bool HandMsgFromGameSrv(uint16, CInputPacket& inPacket);
	bool HandMsgFromUserSrv(uint16, CInputPacket& inPacket);

	void SendData(uint16, COutputPacket& outPacket);
	void RegConnSrv();
	void ConnectSucess(CInputPacket& inPacket);
	void UploadSrvInfo();
private:
	uint16 m_nPeerSerType;
	uint16 m_nPeerSerNo;
	
	CTimer m_timer_Link;
	uint16 m_nTestNum;

	uint16 m_nReConnectCount;
	CTimer m_timer_reconnect;
};

