#pragma once
#include "../Defines.h"
#include "../NetHandSink.h"
#include "../include/TimerNode.h"

class CServices;
class ConnSucess;

class CConnCliSink : public CNetHandSink
{
public:
	CConnCliSink(CServices* pNet);
	~CConnCliSink();
public:
	void Init(uint32 nIp);
	bool DisConnect();
	void Close();
	bool HandNetData(uint16,uint16, uint16, void*, uint16);
	bool HandTimeMsg(uint16 uTimeID);
private:	
	bool HandMsgFromCenterSrv(uint16, uint16, void*, uint16);
	bool HandMsgFromGameSrv(uint16, uint16, void*, uint16);
	bool HandMsgFromUserSrv(uint16, uint16, void*, uint16);

	void SendData(uint16, uint16, uint16, void*, uint16);
	void RegConnSrv();
	void ConnectSucess(ConnSucess* pConn);
private:
	uint16 m_nPeerSerType;
	uint16 m_nPeerSerNo;
	
	uint16 m_nTestLink;
	char m_nReConnectCount;
	CTimerNode m_timerConnTest;
	CTimerNode m_timerReConn;
};

