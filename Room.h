#pragma once
#include <map>
#include "Services.h"
//#include "BaseLogic.h"
#include "commproto.h"
#include "SmartPoint.h"
#include "TimerNode.h"

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
	USHORT m_nUserCount;
	UID m_dwOwerId;
	USHORT m_nAllRound;
	USHORT m_nCurRound;
	bool m_bStarted;
	//Users** m_pUsers;
	//CUserInfo** m_pUsers;
	CSmartArrayPoint<CUserInfo*> m_pUsers;
	USHORT *m_pUserState;
	CBaseLogic* m_pGameLogic;

	
	
	static void* m_pHander;
	static CBaseLogic* (*m_sFun)(CRoom*);
	//char* m_p;
public:
	CRoom();
	~CRoom();
	CBaseLogic* CreateLogic();
	bool InitRoomData(UID uid,int,CreateRoom* pData,USHORT uDataSize);
	static bool LoadGameLogic(int nGameId);
	static void ReleaseGameLogic();
public:
	bool GameStart();
	void GameFinish();
	void SendDataToUser(USHORT, USHORT, USHORT, void* pData = NULL, USHORT nDataSize = 0);
	void SendDataToAll(USHORT, USHORT, void*, USHORT,USHORT nSeatNo = -1);
	USHORT GetUserCount() const;
	CUserInfo* GetUsers(USHORT nSeatNo);

private:
	virtual bool HandData(int e, SERVICEINDEX uFromSerId, void *pData, DATASIZE& size);
	void PreExitSelf();
	
private:
	bool HandNetMsg(USHORT nSeatNo,USHORT nIndex,USHORT uMain,USHORT uSub,void* pData,USHORT uDataSize);
	bool HandTimeMsg(USHORT uTimeId);
	bool HandDataBaseRet();
	bool HandMemDataRet(UINT uType, void* pData, USHORT uDataSize);
	bool HandUserMsg(UINT nType, void* pData, USHORT nDataSize);
	bool UserJoin(USHORT nCsid, USHORT nIndex,void* pData,USHORT uDataSize);
	bool UserQuit(USHORT);
	void GameOver();
	void SendRoomInfoToUser(USHORT nSeatNo);
	void SendAllUsersInfoToUser(USHORT);
	void SyncGameSerInfo(USHORT nSeatNo);
	void UserReady(USHORT nSeatNo);
};

