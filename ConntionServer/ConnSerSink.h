#pragma once


#include "../NetHandSink.h"
#include "../UserInfo.h"
#include "../include/TimerNode.h"
#include "../PacketParse.h"

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
	bool HandNetData(uint16, uint16, CInputPacket& inPacket);
	bool HandTimeMsg(uint16 uTimeId);
	
	bool HandDataBaseRet(uint32 uType, CInputPacket& inPacket);
	bool HandMemDataRet(uint32 uType, CInputPacket& inPacket);
	
	bool HandUserMsg(int nEvent, void * pData, DATASIZE uDataSize);
	void Close();
	void Init(const char* szIp);
	void Connect();
	bool DisConnect();
private:
	bool TestNetLink();

	bool SendToMySelf(COutputPacket& out);
	bool SendToGameSer(uint16, COutputPacket& out);
	bool SendToCenterSer(COutputPacket& out);
	bool SendToUserSer(uint32, COutputPacket& out);
	bool SendToConnectSer(COutputPacket& out);
	
	bool HandMainMsgNet(uint16, CInputPacket& inPacket);
	bool HandMainMsgToRoom(uint16, CInputPacket& inPacket);
	bool HandMainMsgToGame(uint16, uint16, CInputPacket& inPacket);
	bool HandMainMsgFromConnect(uint16 nSub, CInputPacket& inPacket);
	bool HandMainMsgToHall(uint16, uint16, CInputPacket& inPacket);
private:
	int m_nTestLink;
	CTimer m_timerConnTest;
	const char* m_szIp;

	CTimer m_timer_Login;
	
	UID m_nUserId;
	uint16 m_nGameSrvNo;
	uint16 m_nGameSrvIndex;
};

