#pragma once
#include "../include/types.h"
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
	void Init(UINT nIp);
	bool DisConnect();
	void Close();
	bool HandNetData(USHORT,USHORT, USHORT, void*, USHORT);
	bool HandTimeMsg(USHORT uTimeID);
private:	
	bool HandMsgFromCenterSrv(USHORT, USHORT, void*, USHORT);
	bool HandMsgFromGameSrv(USHORT, USHORT, void*, USHORT);
	bool HandMsgFromUserSrv(USHORT, USHORT, void*, USHORT);

	void SendData(USHORT, USHORT, USHORT, void*, USHORT);
	void RegConnSrv();
	void ConnectSucess(ConnSucess* pConn);
private:
	USHORT m_nPeerSerType;
	USHORT m_nPeerSerNo;
	
	USHORT m_nTestLink;
	char m_nReConnectCount;
	CTimerNode m_timerConnTest;
	CTimerNode m_timerReConn;
};

