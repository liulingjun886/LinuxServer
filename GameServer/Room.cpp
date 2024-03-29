#include "Room.h"
#include <stdio.h>
#include <dlfcn.h>
#include "RoomManager.h"
#include "../UserInfo.h"
#include "../NetSinkObj.h"
#include "GameCliSink.h"
#include "../UserServer/MemDataDef.h"
#include "../Game/BaseLogic.h"
#include "GameServer.h"
#include "../include/core/SingleObject.h"


#define GAME_OVER 1
#define GAME_MUST_BEGIN 2
#define GAME_BEGIN 3
#define GAME_FINISH 4


extern CGameServer* g_pGameServer;

void* CRoom::m_pHander = NULL;
CBaseLogic* (*CRoom::m_sFun)(CRoom*) = NULL;



bool CRoom::LoadGameLogic(int nGameId)
{
	char szFileName[32] = { 0 };
	sprintf(szFileName, "./%d.so", nGameId);
	m_pHander = dlopen(szFileName, RTLD_LAZY);
	if(!m_pHander)
		return false;
	
	m_sFun = (CBaseLogic* (*)(CRoom*))dlsym(m_pHander, "CreateLogic");
	if(!m_sFun)
		return false;

	return true;
}

void CRoom::ReleaseGameLogic()
{
	if (m_pHander)
	{
		dlclose(m_pHander);
	}
}


CRoom::CRoom() :m_nRoomId(0)
	,m_nUserCount(0)
	,m_bStarted(false)
	,m_pUserState(NULL)
	,m_pGameLogic(NULL)
{
	m_timerGameOver.InitTimerObj(this, GAME_OVER);
	m_timerGameMustBegin.InitTimerObj(this, GAME_MUST_BEGIN);
	m_timerGameBegin.InitTimerObj(this, GAME_BEGIN);
	m_timerGameFinish.InitTimerObj(this, GAME_FINISH);
}

CRoom::~CRoom()
{
	PreExitSelf();
	SAFE_DELTE(m_pGameLogic);
	SAFE_DELTEARRAY(m_pUserState);
}

CBaseLogic* CRoom::CreateLogic()
{
	return m_sFun(this);
}

bool CRoom::InitRoomData(UID uid,int nRoomId,CreateRoom* pData, uint16 uDataSize)
{
	if (m_nRoomId != 0)
		return false;

	m_nRoomId = nRoomId;
	m_dwOwerId = uid;
	m_nGameId = pData->roominfo.nGameId;
	m_nUserCount = pData->roominfo.uPlayerCount;
	m_nAllRound = pData->roominfo.uRound;
	m_nCurRound = 1;
	m_pGameLogic = CreateLogic();
	if (m_pGameLogic == NULL)
		return false;
	
	//m_pUsers = new CUserInfo*[m_nUserCount];
	m_pUsers.Init(m_nUserCount);
	for (unsigned int i = 0; i < m_pUsers.size(); i++)
	{
		m_pUsers[i] = NULL;
	}
	
	m_pUserState = new uint16[m_nUserCount];
	memset(m_pUserState,0,sizeof(uint16)*m_nUserCount);
	
	if (m_pGameLogic->InitGameStation(&pData->logicinfo, uDataSize - sizeof(CreateRoomInfo)))
	{
		//m_timerGameMustBegin.StartTimerSec(60);
		//Log("房间创建成功,nRoomId = %d", m_nRoomId);
		return true;
	}
	return false;
}

bool CRoom::HandData(int e, SERVICEINDEX uFromSerId, void *pData, DATASIZE nDataSize)
{
	switch (e)
	{
	case USER_NET_MSG:
	{		
		UID nUserId = *(UID*)pData;
		CInputPacket in;
		in.Copy((char*)pData + (uint32)sizeof(UID), nDataSize - (DATASIZE)sizeof(UID));

		if(!HandNetMsg(nUserId, uFromSerId, in.GetMainCmd(), in.GetSubCmd(), in))
		{
			//char buf[MAX_MSG_SIZE] = { 0 };
			//UserGameMsgHead* pHead = (UserGameMsgHead*)buf;
			//pHead->userid = pUserMsg->userid;
			//pHead->uMain = MAIN_ERROR;
			//pHead->uSub = MAIN_USER_MSG_ERROR;
			//SendData(uFromSerId, MAIN_MSG_GAMESER, SUB_MSG_USER, buf, sizeof(UserGameMsgHead));
		}
		return true;
	}
	case TIME_MSG:
	{
		TIMEERID uTimeId = *(TIMEERID*)pData;
		return HandTimeMsg(uTimeId);
	}
	case DATA_BASE_RET:
	{
		return true;
	}
	case MEM_DATA_BASE_RET:
	{
		uint32 uType = *(uint32*)pData;
		return HandMemDataRet(uType, (uint32*)pData + 1,nDataSize - sizeof(uint32));
	}
	case USER_MSG:
	{
		uint32 nType = *(uint32*)pData;
		return HandUserMsg(nType,(uint32*)pData + 1,nDataSize - sizeof(uint32));
	}
	default:
		break;
	}
	return false;
}

void CRoom::PreExitSelf()
{
	for (unsigned int i = 0; i < m_pUsers.size(); i++)
	{
		if (m_pUsers[i])
		{
			delete m_pUsers[i];
		}
	}
	CSingleObject<CRoomManager>::Instance()->DestroyRoom(m_nRoomId);
}

bool CRoom::HandNetMsg(uint16 nSeatNo,uint16 nIndex,uint16 uMain, uint16 uSub, CInputPacket& inPacket)
{
	switch (uMain)
	{
		case MAIN_MSG_ROOM:
		{
			switch (uSub)
			{
			case SUB_MSG_JOIN:
			{
				return UserJoin(nSeatNo, nIndex, inPacket);
			}
			case SUB_MSG_USER_RECONNECT:
			{
				UserReConnInfo* pConn = (UserReConnInfo*)inPacket.ReadBinary(sizeof(UserReConnInfo));
				CUserInfo* pUser = m_pUsers[nSeatNo];

				if(pUser && pConn->nUserId == pUser->GetUserId())
				{
					pUser->UpdateConnInfo(pConn->nCid, pConn->nCsid);
					SendRoomInfoToUser(nSeatNo);
					return true;
				}

				Mem::UserQuitGameReq req;
				req.nUserId = pConn->nUserId;
				//CCliNetSink::PostMemDataBaseReq(this,Mem::USER_QUIT_GAME_REQ, &req, sizeof(Mem::UserQuitGameReq));
				
				InnserSync sync;
				sync.nIndex = pConn->nCsid;
				sync.nType = EN_RECONNECT_FAIL;
				//CNetSinkObj::SendData(this,CGameCliNetSink::GetConnSerById(pConn->nCid,GetServiceIndex()),MAIN_MSG_GAMESER, SUB_MSG_GAME2CONN,  &sync,sizeof(InnserSync));
				return true;
			}
			case SUB_MSG_USER_DISCONNECT:
			{
				CUserInfo* pUser = m_pUsers[nSeatNo];
				pUser->UpdateConnInfo(0, 0);
				//用户断线
				return true;
			}
			case SUB_MSG_USER_READY:
			{
				UserReady(nSeatNo);
				return true;
			}
			default:
			  break;
			}
			break;
		}
		case MAIN_MSG_GAME:
		{
			m_pGameLogic->HandNetMsg(nSeatNo, uSub, inPacket);
			break;
		}
		default:
		{
			break;
		}
		
	}
	return true;
}

bool CRoom::HandTimeMsg(uint16 nTimeId)
{
	switch (nTimeId)
	{
	case GAME_OVER:
	{
		GameOver();
		return false;
	}
	case GAME_MUST_BEGIN:
	{
		m_timerGameMustBegin.StopTimer();
		if (!m_bStarted)
		{
			for(uint32 i = 0; i < m_pUsers.size(); i++)
			{
				if(m_pUsers[i])
					UserQuit(i);
			}
			return false;
		}
		break;
	}
	case GAME_BEGIN:
	{
		for(uint32 i = 0; i < m_pUsers.size(); i++)
		{
			UserReady(i);
		}
		break;
	}
	case GAME_FINISH:
	{
		GameFinish();
		break;
	}
	default:
		return m_pGameLogic->HandTimeMsg(nTimeId);
	}
	return true;
}

bool CRoom::HandMemDataRet(uint32 uType, void* pData, uint16 uDataSize)
{

	switch (uType)
	{
		case Mem::USER_JOIN_GAME_RET:
		{
			Mem::UserJoinGameRet *pRet = (Mem::UserJoinGameRet*)pData;
			CUserInfo* pUser = m_pUsers[pRet->nSeatNo];
			pUser->UpdateConnInfo(pRet->nCid, pRet->nCsid);
			SyncGameSerInfo(pRet->nSeatNo);
			SendRoomInfoToUser(pRet->nSeatNo);
			break;
		}
		default:
			break;
	}
	return true;
}

bool CRoom::HandUserMsg(uint32 nType, void* pData, uint16 nDataSize)
{
	return true;
}


void CRoom::SendDataToUser(uint16 nSeatNo, COutputPacket& outdata)
{
	if(nSeatNo >= m_nUserCount)
		return;
	
	CUserInfo* pUser = m_pUsers[nSeatNo];
	if(!pUser)
		return;
	
	uint16 nCid,nCsid;
	pUser->GetConnInfo(nCid, nCsid);

	COutputPacket out;
	out.Begin(MAIN_MSG_GAMESER, GS_SUB_MSG_GAME2USER);
	out.WriteInt16(nCsid);
	out.WriteInt32(outdata.Packet_Len());
	out.WriteBinary(outdata.Get_Packet(), out.Packet_Len());
	out.End();
	CNetSinkObj::SendData(this,g_pGameServer->GetConnSrvIndex(nCid,GetServiceIndex()+nSeatNo), out);
}

void CRoom::SendDataToAll(COutputPacket& out,uint16 nSeatNo)
{	
	for(int i = 0; i < m_nUserCount; i++)
	{
		if(i == nSeatNo)
			continue;
		SendDataToUser(i,out);
	}
}

uint16 CRoom::GetUserCount() const
{
	return m_nUserCount;
}

CUserInfo* CRoom::GetUsers(uint16 nSeatNo)
{
	if(nSeatNo >= m_pUsers.size())
		return NULL;
	return m_pUsers[nSeatNo];
}

bool CRoom::UserJoin(uint16 nCsid, uint16 nIndex, CInputPacket& in)
{
	//char buff[MAX_MSG_SIZE] = { 0 };
	int nSeatNo = -1;
	UserBaseInfo* pInfo = (UserBaseInfo*)in.ReadBinary((uint32)sizeof(UserBaseInfo));
	for (uint32 i = 0; i < m_pUsers.size(); i++)
	{
		if (m_pUsers[i] == NULL)
		{
			CUserInfo* pUser = new CUserInfo;
			m_pUsers[i] = pUser;
			pUser->SetUserBaseInfo(pInfo->nUserId, pInfo->bSex, pInfo->szName, pInfo->szHeadUrl);
			nSeatNo = i;
			break;
		}
	}
	
	if(nSeatNo >= 0)
	{	
		/*
		static CMutexLock m;
		static int nJoin = 0;
		CToolLock lock(&m);
		printf("nJoin = %d\n",++nJoin);*/
		
		Mem::UserJoinGameReq req;
		req.nUserId = pInfo->nUserId;
		req.nGid = g_pGameServer->GetSerNo();
		req.nGsid = GetServiceIndex();
		req.nSeatNo = nSeatNo;
		//CCliNetSink::PostMemDataBaseReq(this,Mem::USER_JOIN_GAME_REQ, &req, sizeof(Mem::UserJoinGameReq));
	}
	else
	{
		COutputPacket out;
		out.Begin(MAIN_MSG_GAMESER,GS_SUB_MSG_GAME2USER);
		out.End();
		
		char szBuff[MAX_MSG_SIZE] = { 0 };
		Game2User *szData = (Game2User*)szBuff;
		szData->nIndex = nCsid;
		szData->nMain = MAIN_MSG_ROOM;
		szData->nSub = SUB_MSG_JOIN_FAIL;
		//CNetSinkObj::SendData(this,nIndex, MAIN_MSG_GAMESER, SUB_MSG_GAME4USER, szBuff, sizeof(Game2User));
	}
	
	return true;
}

bool CRoom::UserQuit(uint16 nSeatNo)
{
	CUserInfo* pUser = m_pUsers[nSeatNo];
	if(!pUser)
		return true;
	
	m_pUsers[nSeatNo] = NULL;
	m_pUserState[nSeatNo] = 0;
	
	uint16 nCid,nCsid;
	pUser->GetConnInfo(nCid, nCsid);
	Mem::UserQuitGameReq req;
	req.nUserId = pUser->GetUserId();
	delete pUser;

	//CCliNetSink::PostMemDataBaseReq(this,Mem::USER_QUIT_GAME_REQ, &req, sizeof(Mem::UserQuitGameReq));
	
	char buf[MAX_MSG_SIZE] = { 0 };
	InnserSync *pSync = (InnserSync *)buf;
	pSync->nType = EN_SYNC_GAME_SER_INFO;
	pSync->nIndex = nCsid;
	
	UserGameSerInfo *pGame = (UserGameSerInfo *)(pSync+1);
	pGame->nUserId = req.nUserId;
	pGame->nGid = 0;
	pGame->nGsid = 0;
	pGame->nSeatNo = 0;
	//CNetSinkObj::SendData(this,CGameCliNetSink::GetConnSerById(nCid,GetServiceIndex()+nSeatNo), MAIN_MSG_GAMESER,SUB_MSG_GAME2CONN,buf,sizeof(InnserSync)+sizeof(UserGameSerInfo));
	return true;
}

bool CRoom::GameStart()
{
	if (!m_bStarted)
	{
		m_bStarted = true;
	}
	//m_timerGameFinish.StartTimerSec(10);
	return m_pGameLogic->GameBegin();
}

void CRoom::GameFinish()
{
	if (m_nAllRound == m_nCurRound)
	{
		m_timerGameOver.StartTimerSec(5);
		return;
	}

	for(int i= 0; i < m_nUserCount; i++)
	{
		m_pUserState[i] = 0;
	}
	
	++m_nCurRound;
	m_timerGameBegin.StartTimerSec(5);
}

void CRoom::GameOver()
{
	//Log("GameOver nRoomId = %d",m_nRoomId);
	m_pGameLogic->GameOver();
	for (uint32 i = 0; i < m_pUsers.size(); i++)
	{
		if (m_pUsers[i] != NULL)
		{
			UserQuit(i);
		}
	}
}

void CRoom::SendRoomInfoToUser(uint16 nSeatNo)
{
	CUserInfo* pUser = m_pUsers[nSeatNo];
	if(!pUser)
		return;
	
	char buf[MAX_MSG_SIZE] = { 0 };	
	RoomInfo* pRoomInfo = (RoomInfo*)buf;
	pRoomInfo->nGameId = m_nGameId;
	pRoomInfo->nRoomId = m_nRoomId;
	
	pRoomInfo->nUserCount = m_nUserCount;
	pRoomInfo->nRound = m_nAllRound;
	pRoomInfo->nCurRound = m_nCurRound;	

	COutputPacket out;
	out.Begin(MAIN_MSG_ROOM, SUB_MSG_ROOMINFO);
	out.WriteBinary(buf, (uint32)sizeof(RoomInfo));
	out.End();
	SendDataToUser(nSeatNo, out);
}

void CRoom::SendAllUsersInfoToUser(uint16 nSeatNo)
{
	
}


void CRoom::SyncGameSerInfo(uint16 nSeatNo)
{
	CUserInfo *pUser = m_pUsers[nSeatNo];
	if(!pUser)
		return;
	
	uint16 nCid,nCsid;
	pUser->GetConnInfo(nCid, nCsid);
	
	char buf[MAX_MSG_SIZE] = { 0 };
	InnserSync *pSync = (InnserSync *)buf;
	pSync->nType = EN_SYNC_GAME_SER_INFO;
	pSync->nIndex = nCsid;

	UserGameSerInfo *pGame = (UserGameSerInfo *)(pSync+1);
	pGame->nUserId = pUser->GetUserId();
	pGame->nGid = g_pGameServer->GetSerNo();
	pGame->nGsid = GetServiceIndex();
	pGame->nSeatNo = nSeatNo;
	//CNetSinkObj::SendData(this,CGameCliNetSink::GetConnSerById(nCid,GetServiceIndex()+nSeatNo), MAIN_MSG_GAMESER, SUB_MSG_GAME2CONN, buf, 	sizeof(InnserSync)+sizeof(UserGameSerInfo));	
}

void CRoom::UserReady(uint16 nSeatNo)
{
	m_pUserState[nSeatNo] = 1;
	
	for(int i = 0; i < m_nUserCount; i++)
	{
		if(m_pUserState[i] == 0)
			return;
	}
	GameStart();
}

