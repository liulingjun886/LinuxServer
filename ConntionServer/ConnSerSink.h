#pragma once


#include "../NetHandSink.h"
#include "../UserInfo.h"
#include "../include/TimerNode.h"

class CServices;

enum SERVICE_TYPE
{
	TYPE_GAME,
	TYPE_USER,
};

typedef struct tagGameSerInfo
{
	uint32 nSerNo;
	uint32 nGameId;
}GameSerInfo;

union KEY
{
	CUserInfo* pUserInfo;
	GameSerInfo* pSerInfo;
};

class CConnSerSink : public CNetHandSink
{
public:
	CConnSerSink(CServices* m_pNetSer);
	~CConnSerSink();
public:
	bool HandNetData(uint16,uint16, uint16, void*, uint16);
	bool HandTimeMsg(uint16 uTimeId);
	bool HandDataBaseRet(uint32 uType, void * pData, uint16 nDataSize);
	bool HandMemDataRet(uint32 uType, void* pData, uint16 uDataSize);
	bool HandUserMsg(int nEvent, void * pData, uint16 uDataSize);
	void Close();
	void Init(const char* szIp);
	void Connect();
	bool DisConnect();
private:
	bool TestNetLink();

	bool SendToMySelf(uint16, uint16, void* pData = NULL, uint16 nDataSize = 0);
	bool SendToGameSer(uint16, uint16, uint16, void*, uint16);
	bool SendToCenterSer(uint16, uint16, void*, uint16);
	bool SendToUserSer(uint32, uint16, uint16, void*, uint16);
	bool SendToConnectSer(uint16, uint16, void*, uint16);
	
	bool HandMainMsgNet(uint16, uint16, void*, uint16);
	bool HandMainMsgToRoom(uint16, uint16, void*, uint16);
	bool HandMainMsgToGame(uint16,uint16, uint16, void*, uint16);
	bool HandMainMsgFromConnect(uint16 nIndex,uint16 nSub, void* pData, uint16 nDataSize);
private:
	CUserInfo* m_pUserInfo;
	int m_nTestLink;
	const char* m_szIp;
	CTimerNode m_timerConnTest;
};

