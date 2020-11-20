#pragma once

#include "NetGameSerManager.h"
#include "NetHandSink.h"
#include "UserInfo.h"
#include "TimerNode.h"

class CServices;

enum SERVICE_TYPE
{
	TYPE_GAME,
	TYPE_USER,
};

typedef struct tagGameSerInfo
{
	UINT nSerNo;
	UINT nGameId;
}GameSerInfo;

union KEY
{
	CUserInfo* pUserInfo;
	GameSerInfo* pSerInfo;
};

class CConnSerNetSink : public CNetHandSink
{
public:
	CConnSerNetSink(CServices* m_pNetSer);
	~CConnSerNetSink();
public:
	bool HandNetData(USHORT,USHORT, USHORT, void*, USHORT);
	bool HandTimeMsg(USHORT uTimeId);
	bool HandDataBaseRet(UINT uType, void * pData, USHORT nDataSize);
	bool HandMemDataRet(UINT uType, void* pData, USHORT uDataSize);
	bool HandUserMsg(int nEvent, void * pData, USHORT uDataSize);
	void Close();
	void Init(const char* szIp);
	void Connect();
	bool DisConnect();
private:
	bool TestNetLink();
	bool SendToGameSer(USHORT, USHORT, USHORT, void*, USHORT);
	bool SendToCenterSer(USHORT, USHORT, USHORT, void*, USHORT);
	bool SendToUserSer(UINT, USHORT, USHORT, void*, USHORT);
	bool HandMainMsgNet(USHORT, USHORT, void*, USHORT);
	
	bool HandMainMsgToRoom(USHORT, USHORT, void*, USHORT);
	bool HandMainMsgToGame(USHORT,USHORT, USHORT, void*, USHORT);
	
	bool HandMainMsgFromConnect(USHORT nIndex,USHORT nSub, void* pData, USHORT nDataSize);
private:
	CUserInfo* m_pUserInfo;
	int m_nTestLink;
	const char* m_szIp;
	CTimerNode m_timerConnTest;
};

