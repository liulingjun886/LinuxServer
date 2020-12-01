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
	bool HandNetData(uint16,uint16, uint16, void*, DATASIZE);
	bool HandTimeMsg(uint16 uTimeId);
	bool HandDataBaseRet(uint32 uType, void * pData, DATASIZE nDataSize);
	bool HandMemDataRet(uint32 uType, void* pData, DATASIZE uDataSize);
	bool HandUserMsg(int nEvent, void * pData, DATASIZE uDataSize);
	void Close();
	void Init(const char* szIp);
	void Connect();
	bool DisConnect();
private:
	bool TestNetLink();

	bool SendToMySelf(uint16, uint16, void* pData = NULL, DATASIZE nDataSize = 0);
	bool SendToGameSer(uint16, uint16, uint16, void*, DATASIZE);
	bool SendToCenterSer(uint16, uint16, void*, DATASIZE);
	bool SendToUserSer(uint32, uint16, uint16, void*, DATASIZE);
	bool SendToConnectSer(uint16, uint16, void*, DATASIZE);
	
	bool HandMainMsgNet(uint16, uint16, void*, DATASIZE);
	bool HandMainMsgToRoom(uint16, uint16, void*, DATASIZE);
	bool HandMainMsgToGame(uint16,uint16, uint16, void*, DATASIZE);
	bool HandMainMsgFromConnect(uint16 nIndex,uint16 nSub, void* pData, DATASIZE nDataSize);
private:
	int m_nTestLink;
	CTimer m_timerConnTest;
	const char* m_szIp;
	
	CUserInfo* m_pUserInfo;
	UID m_nUserId;
	
	
	
};

