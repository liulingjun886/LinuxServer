#include <stdio.h>
#include "ConnSerNetSink.h"
#include "Core.h"
#include "ToolLock.h"
#include "commproto.h"
#include "Services.h"
#include "MemDataDef.h"
#include "DataBaseDef.h"
#include "NetSink.h"
#include "CliNetSink.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


extern USHORT g_serno;
CGameSerManager CConnSerNetSink::m_RemoteSer;
CConnSerNetSink::CConnSerNetSink(CServices* pNet) :CNetHandSink(pNet),m_nNetType(TYPE_USER),m_nTestLink(0)
{
	m_nKey.pUserInfo = NULL;
	m_timerConnTest.InitTimerObj(m_pNet, TIME_CONN_IS_LINK);
}


CConnSerNetSink::~CConnSerNetSink()
{
	if(m_nNetType == TYPE_USER)
	{
		if(m_nKey.pUserInfo)
			delete m_nKey.pUserInfo;
	}
	else
	{
		if(m_nKey.pSerInfo)
			delete m_nKey.pSerInfo;
	}
	m_nKey.pUserInfo = NULL;
}

void CConnSerNetSink::Init(UINT nIp)
{
	m_nIp = nIp;
	//m_pNet->SetTimer(TIME_CONN_IS_LINK, 200, -1);
}

void CConnSerNetSink::Init(const char* szIp)
{
	m_szIp = szIp;
}

void CConnSerNetSink::Connect()
{
	//m_pNet->SetTimer(TIME_CONN_IS_LINK, 100*60, -1);
	CNetSink::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_CONN_SUCSS);
}

bool CConnSerNetSink::DisConnect()
{
	if(m_nNetType == TYPE_USER && m_nKey.pUserInfo)
	{
		Mem::UserLogOutMemReq req;
		req.nUserId = m_nKey.pUserInfo->GetUserId();
		req.nSerNo = g_serno;
		req.nSockNo = m_pNet->GetServiceIndex();
		CCliNetSink::PostMemDataBaseReq(m_pNet,Mem::USER_LOGOUT_REQ, &req, sizeof(Mem::UserLogOutMemReq));

		USHORT nGid,nGsid,nSeatNo;
		m_nKey.pUserInfo->GetGameInfo(nGid,nGsid,nSeatNo);
		
		if(nGid>0)
		{
			User2Game pUg;
			pUg.nMain = MAIN_MSG_ROOM;
			pUg.nSub = SUB_MSG_USER_DISCONNECT;
			pUg.nIndex = nGsid;
			pUg.nSeatNo = nSeatNo;
			USHORT nIndex = m_RemoteSer.GetSerBySerNo(nGid,m_pNet->GetServiceIndex());
			SendToGameSer(nIndex, MAIN_MSG_CONNECT, SUB_MSG_USER4GAME,&pUg,sizeof(User2Game));
		}
	}
	else if(m_nNetType == TYPE_GAME)
	{	
		printf("Game Ser DisConnect\n");
		m_RemoteSer.DelRemoteSer(m_nKey.pSerInfo->nSerNo,m_nKey.pSerInfo->nGameId,m_pNet->GetServiceIndex());
		
		Single_Get(CCore)->Stop();
	}
	return false;
}


bool CConnSerNetSink::HandNetData(USHORT nIndex,USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{

	switch (nMain)
	{
	case MAIN_MSG_NET:
	{
		return HandMainMsgNet(nIndex,nSub,pData,nDataSize);
	}
	case MAIN_MSG_LOGIN:
	{
		if(nDataSize != sizeof(DataBase::UserLoginReq))
			return false;
		if(m_nKey.pUserInfo)
			return false;
		//DataBase::UserLoginReq* pReq = (DataBase::UserLoginReq*)pData;
		CCliNetSink::PostDataBaseReq(m_pNet,DataBase::USER_LOGIN_REQ,pData,nDataSize);
		return true;
	}
	case MAIN_MSG_ROOM_MANAGER:
	{
		return HandMainMsgToRoom(nIndex,nSub,pData,nDataSize);
	}
	case MAIN_MSG_ROOM:
	case MAIN_MSG_GAME:
	{
		return HandMainMsgToGame(nIndex,nMain,nSub,pData,nDataSize);
	}
	case MAIN_MSG_GAMESER:
	{
		return HandMainMsgFromGame(nIndex,nMain,nSub,pData,nDataSize);
	}
	case MAIN_MSG_CONNECT:
	{
		return HandMainMsgFromConnect(nIndex,nSub,pData,nDataSize);
	}
	case MAIN_ERROR:
	{
		return false;
	}
	default:
		break;
	}
	return false;
}

bool CConnSerNetSink::HandTimeMsg(USHORT uTimeId)
{
	switch (uTimeId)
	{
	case TIME_CONN_IS_LINK:
	{
		return TestNetLink();
	}
	default:
		break;
	}
	return true;
}

void CConnSerNetSink::Close()
{
	//if(m_nNetType == TYPE_GAME)
	//{	
	//	printf("Game Ser DisConnect\n");
	//	m_RemoteSer.DelRemoteSer(m_nKey.nSerNo);
	//	Single_Get(CCore)->Stop();
	//}
}

bool CConnSerNetSink::TestNetLink()
{
	if (m_nTestLink > 0)
	{
		printf("TestNetLink\n");
	  	return DisConnect();
	}
	++m_nTestLink;
	CNetSink::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_TEST);
	m_timerConnTest.StartTimerSec(60);
	return true;
}


bool CConnSerNetSink::SendToGameSer(USHORT nSerNo, USHORT uMain, USHORT uSub, void* pData, USHORT uDataSize)
{
	CNetSink::SendData(m_pNet,nSerNo, uMain, uSub, pData, uDataSize);
	return true;
}

bool CConnSerNetSink::HandDataBaseRet(UINT nType, void * pData, USHORT nDataSize)
{
	switch (nType)
	{
		case DataBase::USER_LOGIN_RET:
		{
			DataBase::UserLoginRet* pRet = (DataBase::UserLoginRet*)pData;
			
			m_nKey.pUserInfo = new CUserInfo;
			m_nKey.pUserInfo->SetUserBaseInfo(pRet->nUserId, pRet->nSex, pRet->szName, pRet->szHeadUrl);
			m_nKey.pUserInfo->UpdateConnInfo(g_serno, m_pNet->GetServiceIndex());

			Mem::UserLoginMemReq req;
			req.nUserId = m_nKey.pUserInfo->GetUserId();
			req.nSerNo = g_serno;
			req.nSockNo = m_pNet->GetServiceIndex();
			CCliNetSink::PostMemDataBaseReq(m_pNet,Mem::USER_LOGIN_REQ, &req, sizeof(req));
			break;
		}
		default:
			return false;
	}
	return true;
}

bool CConnSerNetSink::HandMemDataRet(UINT nType, void* pData, USHORT uDataSize)
{
	switch (nType)
	{
		case Mem::USER_LOGIN_RET:
		{
			Mem::UserLoginMemRet* pRet = (Mem::UserLoginMemRet*)pData;
			m_nKey.pUserInfo->UpdateGameInfo(pRet->nGid, pRet->nGSid, pRet->nGsno);
			CNetSink::SendData(m_pNet,m_pNet->GetServiceIndex(),MAIN_MSG_LOGIN,SUB_MSG_LOGIN,  m_nKey.pUserInfo->GetUserBaseInfo(),sizeof(UserBaseInfo));
			
			if(pRet->nCid > 0)
			{
				
				if(pRet->nCid == g_serno)
				{
					UserDoubleLogin login;
					login.nUserId = m_nKey.pUserInfo->GetUserId();
					login.nCid = pRet->nCid;
					login.nCsid = pRet->nCSid;
					m_pNet->PostData(pRet->nCSid, EN_DOUBLE_LOGIN, &login, sizeof(UserDoubleLogin));
				}
				else
				{
					UserDoubleLogin login;
					login.nUserId = m_nKey.pUserInfo->GetUserId();
					login.nCid = pRet->nCid;
					login.nCsid = pRet->nCSid;
					USHORT nIndex = m_RemoteSer.RandSer(m_pNet->GetServiceIndex());
					SendToGameSer(nIndex, MAIN_MSG_CONNECT, SUB_MSG_USER_DOUBLELOGIN, &login,sizeof(UserDoubleLogin));
				}
			}

			if(pRet->nGid > 0)
			{
				char szBuff[MAX_MSG_SIZE] = {0};
				User2Game* pUg = (User2Game*)szBuff;
				pUg->nMain = MAIN_MSG_ROOM;
				pUg->nSub = SUB_MSG_USER_RECONNECT;
				pUg->nIndex = pRet->nGSid;
				pUg->nSeatNo = pRet->nGsno;
				
				UserReConnInfo* pConn = (UserReConnInfo*)(pUg+1);
				pConn->nUserId = m_nKey.pUserInfo->GetUserId();
				pConn->nCid = g_serno;
				pConn->nCsid = m_pNet->GetServiceIndex();

				USHORT nIndex = m_RemoteSer.GetSerBySerNo(pRet->nGid,m_pNet->GetServiceIndex());
				SendToGameSer(nIndex, MAIN_MSG_CONNECT, SUB_MSG_USER_RELOGIN, szBuff, 	sizeof(User2Game)+sizeof(UserReConnInfo));
			}
			break;
		}
		default:
		{
			break;
		}
	}
	return true;
}

bool CConnSerNetSink::HandUserMsg(int nEvent, void * pData, USHORT nDataSize)
{

	switch (nEvent)
	{
		case DATA_BASE_RET:
		{
			UINT *pType = (UINT*)pData;
			return HandDataBaseRet(*pType, pType+1,  nDataSize-sizeof(UINT));
		}
		case MEM_DATA_BASE_RET:
		{
			UINT *pType = (UINT*)pData;
			return HandMemDataRet(*pType, pType+1,  nDataSize-sizeof(UINT));
		}
		case EN_DOUBLE_LOGIN:
		{
			UID *pUid = (UID*)(pData);
			if(m_nNetType == TYPE_USER && m_nKey.pUserInfo && *pUid == m_nKey.pUserInfo->GetUserId())
			{
				CNetSink::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_LOGIN, SUB_MSG_DOUBLE_LOGIN);
				m_pNet->PostData(m_pNet->GetServiceIndex(), EXIT_MSG);
			}
			break;
		}
		case EN_SYNC_GAME_SER_INFO:
		{
			UserGameSerInfo* pInfo = (UserGameSerInfo*)(pData);
			if(m_nNetType == TYPE_USER && m_nKey.pUserInfo && m_nKey.pUserInfo->GetUserId() == pInfo->nUserId)
			{
				m_nKey.pUserInfo->UpdateGameInfo(pInfo->nGid, pInfo->nGsid, pInfo->nSeatNo);
			}
			break;
		}
		case EN_RECONNECT_FAIL:
		{
			Mem::UserQuitGameReq req;
			req.nUserId = m_nKey.pUserInfo->GetUserId();
			CCliNetSink::PostMemDataBaseReq(m_pNet,Mem::USER_QUIT_GAME_REQ, &req, sizeof(Mem::UserQuitGameReq));
			m_nKey.pUserInfo->UpdateGameInfo(0, 0, 0);
			break;
		}
		default:
			return false;
	}	
	return true;
}


bool CConnSerNetSink::HandMainMsgNet(USHORT nIndex,USHORT nSub, void* pData, USHORT nDataSize)
{
	switch (nSub)
	{
		 case SUB_MSG_REG_GAMESER:
		 {
			 RegGameSer* pGameSer = (RegGameSer*)pData;
			 if(!m_RemoteSer.AddRemoteSer(pGameSer->nGameID, pGameSer->nSerNo, nIndex))
			 {
			 	return false;
			 }
			 //m_pNet->PriorityService();
			 m_nNetType = TYPE_GAME;
			 m_nKey.pSerInfo = new GameSerInfo;
			 m_nKey.pSerInfo->nSerNo = pGameSer->nSerNo;
			 m_nKey.pSerInfo->nGameId = pGameSer->nGameID;
			 
			 //m_pNet->SetTimer(TIME_CONN_IS_LINK, 6000, -1);
			 m_timerConnTest.StartTimerSec(60);
			 RegConnSer reg;
			 reg.nSerNo = g_serno;
			 CNetSink::SendData(m_pNet,nIndex, MAIN_MSG_NET, SUB_MSG_REG_CONNSER, &reg,sizeof(RegConnSer));
			 return true;
		 }
		 case SUB_MSG_TEST:
		 {
			m_nTestLink = 0;
			return true;
		 }
		 default:
			 break;
	}
	return false;
}

bool CConnSerNetSink::HandMainMsgToRoom(USHORT nIndex,USHORT nSub, void* pData, USHORT nDataSize)
{
	USHORT nGid, nGsid,nSeatNo;
	m_nKey.pUserInfo->GetGameInfo(nGid, nGsid,nSeatNo);
	if(nGid>0)
	{
		return true;
	}
	
	char buff[MAX_MSG_SIZE] = {0};
	User2Room* pBuff = (User2Room*)buff;
	pBuff->nMain = MAIN_MSG_GAMESER;
	pBuff->nSub = nSub;
	pBuff->nIndex = m_pNet->GetServiceIndex();
	UserBaseInfo* pUserInfo = (UserBaseInfo*)(pBuff+1);
	memcpy(pUserInfo,m_nKey.pUserInfo->GetUserBaseInfo(),sizeof(UserBaseInfo));
	
	USHORT nSerNo = 0;
	if(SUB_MSG_CREATE == nSub)
	{
		CreateRoom* pCreate = (CreateRoom*)pData;
		nSerNo = m_RemoteSer.RandSerByGameId(pCreate->roominfo.nGameId,m_pNet->GetServiceIndex());
	}
	else if(SUB_MSG_ENTER == nSub)
	{
		int nRoomId = *(int*)pData;
		nSerNo = m_RemoteSer.GetSerBySerNo(nRoomId / 100000,m_pNet->GetServiceIndex());
	}
	else
	{
		return true;
	}
	
	if(nDataSize > 0)
	{
		memcpy(pUserInfo+1,pData,nDataSize);
	}
	
	if(nSerNo == INVALID_SERIVCE_INDEX)
		printf("nSerNo falt\n");
	
	SendToGameSer(nSerNo, MAIN_MSG_CONNECT, SUB_MSG_USER4ROOM, buff, nDataSize + sizeof(User2Room) + sizeof(UserBaseInfo));
	return true;
}

bool CConnSerNetSink::HandMainMsgToGame(USHORT nIndex,USHORT nMain,USHORT nSub, void* pData, USHORT nDataSize)
{
	if(m_nNetType != TYPE_USER)
		return false;
	USHORT nGameSerNo,nGameSerIndex,nGameSerSeatNo;
	m_nKey.pUserInfo->GetGameInfo(nGameSerNo,nGameSerIndex,nGameSerSeatNo);
	char buff[MAX_MSG_SIZE] = { 0 };
	User2Game* pCom = (User2Game*)buff;
	pCom->nIndex = nGameSerIndex;
	pCom->nSeatNo = nGameSerSeatNo;
	pCom->nMain = nMain;
	pCom->nSub = nSub;
	memcpy(pCom + 1, pData, nDataSize);
	USHORT nSerIndex = m_RemoteSer.GetSerBySerNo(nGameSerNo,m_pNet->GetServiceIndex());	
	
	SendToGameSer(nSerIndex, MAIN_MSG_CONNECT, SUB_MSG_USER4GAME, buff, nDataSize + sizeof(User2Game));
	return true;
}

bool CConnSerNetSink::HandMainMsgFromGame(USHORT nIndex,USHORT nMain,USHORT nSub, void* pData, USHORT nDataSize)
{
	m_nTestLink = 0;
	switch (nSub)
	{
		case SUB_MSG_GAME4USER:
		{
			Game2User* pBuff = (Game2User*)pData;
			
			//if(pBuff->nMain == 6 && pBuff->nSub == 10)
			//{
				//static CMutexLock m;
				//static int nGameOver = 0;
				//CToolLock lock(&m);
				//m_pNet->Log("nGameOver = %d",++nGameOver);
				//printf("Recv Game Over Msg %d\n",m_pNet->GetServiceIndex());
			//}
			CNetSink::SendData(m_pNet,pBuff->nIndex, pBuff->nMain, pBuff->nSub, pBuff+1, nDataSize-sizeof(Game2User));
			break;
		}
		case SUB_MSG_GAME2CONN:
		{
			InnserSync* pSync = (InnserSync*)pData;
			m_pNet->PostData(pSync->nIndex, pSync->nType, pSync+1, nDataSize-sizeof(InnserSync));
			break;
		}
		default:
			break;
	}
	return true;
}

bool CConnSerNetSink::HandMainMsgFromConnect(USHORT nIndex,USHORT nSub, void* pData, USHORT nDataSize)
{
	switch (nSub)
	{
		case SUB_MSG_USER_DOUBLELOGIN:
		{
			UserDoubleLogin* pDouble = (UserDoubleLogin*)pData;
			m_pNet->PostData(pDouble->nCsid,EN_DOUBLE_LOGIN,&pDouble->nUserId,sizeof(UID));
			break;
		}
		default:
			break;
	}
	return true;
}


