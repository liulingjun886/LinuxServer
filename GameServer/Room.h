#pragma once
#include <map>
#include "../include/core/Services.h"
//#include "BaseLogic.h"
#include "../commproto.h"
#include "../include/core/SmartPoint.h"
#include "../include/core/TimerNode.h"
#include "../PacketParse.h"

class CUserInfo;
class CBaseLogic;

/*
enum EN_USER_MSG_ROOM
{
	NET_MSG=1,
};*/

class CRoom :
	public CServices
{
	CTimerNode m_timerGameOver;
	CTimerNode m_timerGameMustBegin;
	CTimerNode m_timerGameBegin;
	CTimerNode m_timerGameFinish;
	
	int m_nRoomId;
	int m_nGameId;	
	uint16 m_nUserCount;
	UID m_dwOwerId;
	uint16 m_nAllRound;
	uint16 m_nCurRound;
	bool m_bStarted;
	CSmartArrayPoint<CUserInfo*> m_pUsers;
	uint16 *m_pUserState;
	CBaseLogic* m_pGameLogic;
	
	static void* m_pHander;
	static CBaseLogic* (*m_sFun)(CRoom*);
public:
	CRoom();
	~CRoom();
	CBaseLogic* CreateLogic();
	bool InitRoomData(UID uid,int,CreateRoom* pData,uint16 uDataSize);
	static bool LoadGameLogic(int nGameId);
	static void ReleaseGameLogic();
public:
	bool GameStart();
	void GameFinish();
	void SendDataToUser(uint16, COutputPacket&);
	void SendDataToAll(COutputPacket&,uint16 nSeatNo = -1);
	uint16 GetUserCount() const;
	CUserInfo* GetUsers(uint16 nSeatNo);

private:
	virtual bool HandData(int e, SERVICEINDEX uFromSerId, void *pData, DATASIZE size);
	void PreExitSelf();
	
private:
	bool HandNetMsg(uint16 nSeatNo,uint16 nIndex,uint16 uMain,uint16 uSub,CInputPacket& inPacket);
	bool HandTimeMsg(uint16 uTimeId);
	bool HandDataBaseRet();
	bool HandMemDataRet(uint32 uType, void* pData, uint16 uDataSize);
	bool HandUserMsg(uint32 nType, void* pData, uint16 nDataSize);
	bool UserJoin(uint16 nCsid, uint16 nIndex, CInputPacket& in);
	bool UserQuit(uint16);
	void GameOver();
	void SendRoomInfoToUser(uint16 nSeatNo);
	void SendAllUsersInfoToUser(uint16);
	void SyncGameSerInfo(uint16 nSeatNo);
	void UserReady(uint16 nSeatNo);
};

