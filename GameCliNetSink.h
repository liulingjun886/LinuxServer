#pragma once
#include "NetConnSerManager.h"
#include "CliNetSink.h"
#include "TimerNode.h"
enum CONN_TYPE
{
	DATA_SER,
	CONN_SER
};

class CServices;

class CGameCliNetSink : public CCliNetSink
{
	static CConnSerManager m_RemoteSer;
	USHORT m_nTestLink;
	char m_nReConnectCount;
	CONN_TYPE m_nType;
	CTimerNode m_timerConnTest;
	CTimerNode m_timerReConn;
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
public:
	static USHORT GetConnSerById(USHORT,USHORT);
};

