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
	void Init(const char* szIp);
	bool DisConnect();
	void Close();
	bool HandNetData(uint16,uint16, uint16, void*, DATASIZE);
	bool HandTimeMsg(uint16 uTimeID);
private:	
	bool HandMsgFromCenterSrv(uint16, uint16, void*, DATASIZE);
	bool HandMsgFromGameSrv(uint16, uint16, void*, DATASIZE);
	bool HandMsgFromUserSrv(uint16, uint16, void*, DATASIZE);

	void SendData(uint16, uint16, uint16, void*, DATASIZE);
	void RegConnSrv();
	void ConnectSucess(ConnSucess* pConn);
private:
	uint16 m_nPeerSerType;
	uint16 m_nPeerSerNo;
	
	CTimer m_timer_Link;
	uint16 m_nTestNum;

	uint16 m_nReConnectCount;
	CTimer m_timer_reconnect;
};

