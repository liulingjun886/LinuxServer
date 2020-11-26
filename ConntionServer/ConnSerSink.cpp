#include <stdio.h>
#include "ConnSerSink.h"
#include "../include/Core.h"
#include "../include/ToolLock.h"
#include "../commproto.h"
#include "../include/Services.h"
#include "../MemDataBaseEnginer/MemDataDef.h"
#include "../DataBaseEnginer/DataBaseDef.h"
#include "../NetSinkObj.h"
//#include "CliNetSink.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ConnectServer.h"


extern CConnectServer* g_pConnectServer;

//CGameSerManager CConnSerSink::m_RemoteSer;
CConnSerSink::CConnSerSink(CServices* pNet) :CNetHandSink(pNet),m_nTestLink(0)
{
	m_pUserInfo = NULL;
	m_timerConnTest.InitTimerObj(m_pNet, TIME_CONN_IS_LINK);
}


CConnSerSink::~CConnSerSink()
{

	if(m_pUserInfo)
		delete m_pUserInfo;
	
	m_pUserInfo = NULL;
}

void CConnSerSink::Init(const char* szIp)
{
	m_szIp = szIp;
	//m_pNet->SetTimer(TIME_CONN_IS_LINK, 200, -1);
}

void CConnSerSink::Connect()
{
	//m_pNet->SetTimer(TIME_CONN_IS_LINK, 100*60, -1);
	SendToMySelf(MAIN_MSG_CONNSER, CS_SUB_MSG_CONN_SUCSS);
}

bool CConnSerSink::DisConnect()
{

	if(NULL == m_pUserInfo)
		return false;

	Mem::UserLogOutMemReq req;
	req.nUserId = m_pUserInfo->GetUserId();
	req.nSerNo = g_pConnectServer->GetSerNo();
	req.nSockNo = m_pNet->GetServiceIndex();
	//CCliNetSink::PostMemDataBaseReq(m_pNet,Mem::USER_LOGOUT_REQ, &req, sizeof(Mem::UserLogOutMemReq));

	USHORT nGid,nGsid,nSeatNo;
	m_pUserInfo->GetGameInfo(nGid,nGsid,nSeatNo);
	
	if(nGid>0)
	{
		User2Game pUg;
		pUg.nMain = MAIN_MSG_ROOM;
		pUg.nSub = SUB_MSG_USER_DISCONNECT;
		pUg.nIndex = nGsid;
		pUg.nSeatNo = nSeatNo;
		SendToGameSer(nGid, MAIN_MSG_CONNSER, SUB_MSG_USER4GAME,&pUg,sizeof(User2Game));
	}
	return false;
}


bool CConnSerSink::HandNetData(USHORT nIndex,USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{

	switch (nMain)
	{
	//case MAIN_MSG_NET:
	//{
	//	return HandMainMsgNet(nIndex,nSub,pData,nDataSize);
	//}
	//case MAIN_MSG_LOGIN:
	//{
	//	if(nDataSize != sizeof(DataBase::UserLoginReq))
	//		return false;
	//	if(NULL != m_pUserInfo)
	//		return false;
		//DataBase::UserLoginReq* pReq = (DataBase::UserLoginReq*)pData;
		//CCliNetSink::PostDataBaseReq(m_pNet,DataBase::USER_LOGIN_REQ,pData,nDataSize);
	//	return true;
	//}
	//case MAIN_MSG_ROOM_MANAGER:
	//{
	//	return HandMainMsgToRoom(nIndex,nSub,pData,nDataSize);
	//}
	case MAIN_MSG_ROOM:
	case MAIN_MSG_GAME:
	{
		return HandMainMsgToGame(nIndex,nMain,nSub,pData,nDataSize);
	}
	//case MAIN_MSG_CONNECT:
	//{
	//	return HandMainMsgFromConnect(nIndex,nSub,pData,nDataSize);
	//}
	//case MAIN_ERROR:
	//{
	//	return false;
	//}
	default:
		break;
	}
	return false;
}

bool CConnSerSink::HandTimeMsg(USHORT uTimeId)
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

void CConnSerSink::Close()
{
	
}

bool CConnSerSink::TestNetLink()
{
	if (m_nTestLink > 0)
	{
		printf("TestNetLink\n");
	  	return DisConnect();
	}
	++m_nTestLink;
	SendToMySelf(MAIN_MSG_CONNSER, CS_SUB_MSG_TEST);
	m_timerConnTest.StartTimerSec(60);
	return true;
}

bool CConnSerSink::SendToCenterSer(USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	CNetSinkObj::SendData(m_pNet, g_pConnectServer->GetCenterServerIndex(), nMain, nSub, pData, nDataSize);
	return true;
}

bool CConnSerSink::SendToMySelf(USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), nMain, nSub, pData, nDataSize);
	return true;
}

bool CConnSerSink::SendToGameSer(USHORT nSerNo, USHORT uMain, USHORT uSub, void* pData, USHORT uDataSize)
{
	USHORT nIndex = g_pConnectServer->GetGameSerIndexByNo(nSerNo, m_pNet->GetServiceIndex());
	CNetSinkObj::SendData(m_pNet,nIndex, uMain, uSub, pData, uDataSize);
	return true;
}

bool CConnSerSink::SendToUserSer(UINT nUid, USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	USHORT nIndex = g_pConnectServer->GetUserServerIndex(nUid);
	CNetSinkObj::SendData(m_pNet, nIndex, nMain, nSub, pData, nDataSize);
	return true;
}

bool CConnSerSink::SendToConnectSer(USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	USHORT nTempGameSerNo = g_pConnectServer->GetARandGameSer();
	return SendToGameSer(nTempGameSerNo, nMain, nSub, pData, nDataSize);
}


bool CConnSerSink::HandDataBaseRet(UINT nType, void * pData, USHORT nDataSize)
{
	switch (nType)
	{
		case DataBase::USER_LOGIN_RET:
		{
			DataBase::UserLoginRet* pRet = (DataBase::UserLoginRet*)pData;
			
			m_pUserInfo = new CUserInfo;
			m_pUserInfo->SetUserBaseInfo(pRet->nUserId, pRet->nSex, pRet->szName, pRet->szHeadUrl);
			m_pUserInfo->UpdateConnInfo(g_pConnectServer->GetSerNo(), m_pNet->GetServiceIndex());

			Mem::UserLoginMemReq req;
			req.nUserId = m_pUserInfo->GetUserId();
			req.nSerNo = g_pConnectServer->GetSerNo();
			req.nSockNo = m_pNet->GetServiceIndex();
			//CCliNetSink::PostMemDataBaseReq(m_pNet,Mem::USER_LOGIN_REQ, &req, sizeof(req));
			break;
		}
		default:
			return false;
	}
	return true;
}

bool CConnSerSink::HandMemDataRet(UINT nType, void* pData, USHORT uDataSize)
{
	switch (nType)
	{
		case Mem::USER_LOGIN_RET:
		{
			Mem::UserLoginMemRet* pRet = (Mem::UserLoginMemRet*)pData;
			m_pUserInfo->UpdateGameInfo(pRet->nGid, pRet->nGSid, pRet->nGsno);
			SendToMySelf(MAIN_MSG_CONNSER,SUB_MSG_LOGIN, m_pUserInfo->GetUserBaseInfo(),sizeof(UserBaseInfo));
			
			if(pRet->nCid > 0)
			{
				
				if(pRet->nCid == g_pConnectServer->GetSerNo())
				{
					UserDoubleLogin login;
					login.nUserId = m_pUserInfo->GetUserId();
					login.nCid = pRet->nCid;
					login.nCsid = pRet->nCSid;
					m_pNet->PostData(pRet->nCSid, EN_DOUBLE_LOGIN, &login, sizeof(UserDoubleLogin));
				}
				else
				{
					UserDoubleLogin login;
					login.nUserId = m_pUserInfo->GetUserId();
					login.nCid = pRet->nCid;
					login.nCsid = pRet->nCSid;
					//USHORT nIndex = g_pConnectServer->GetCenterServerIndex();
					//SendToCenterSer(MAIN_MSG_CONNECT, SUB_MSG_USER_DOUBLELOGIN, &login, sizeof(UserDoubleLogin));
					SendToConnectSer(MAIN_MSG_CONNSER, SUB_MSG_USER_DOUBLELOGIN, &login, sizeof(UserDoubleLogin));
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
				pConn->nUserId = m_pUserInfo->GetUserId();
				pConn->nCid = g_pConnectServer->GetSerNo();
				pConn->nCsid = m_pNet->GetServiceIndex();
				SendToGameSer(pRet->nGid, MAIN_MSG_CONNSER, SUB_MSG_USER_RELOGIN, szBuff, 	sizeof(User2Game)+sizeof(UserReConnInfo));
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

bool CConnSerSink::HandUserMsg(int nEvent, void * pData, USHORT nDataSize)
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
			if(m_pUserInfo && *pUid == m_pUserInfo->GetUserId())
			{
				SendToMySelf(MAIN_MSG_CONNSER, SUB_MSG_DOUBLE_LOGIN);
				m_pNet->PostData(m_pNet->GetServiceIndex(), EXIT_MSG);
			}
			break;
		}
		case EN_SYNC_GAME_SER_INFO:
		{
			UserGameSerInfo* pInfo = (UserGameSerInfo*)(pData);
			if(m_pUserInfo && m_pUserInfo->GetUserId() == pInfo->nUserId)
			{
				m_pUserInfo->UpdateGameInfo(pInfo->nGid, pInfo->nGsid, pInfo->nSeatNo);
			}
			break;
		}
		case EN_RECONNECT_FAIL:
		{
			Mem::UserQuitGameReq req;
			req.nUserId = m_pUserInfo->GetUserId();
			//CCliNetSink::PostMemDataBaseReq(m_pNet,Mem::USER_QUIT_GAME_REQ, &req, sizeof(Mem::UserQuitGameReq));
			m_pUserInfo->UpdateGameInfo(0, 0, 0);
			break;
		}
		default:
			return false;
	}	
	return true;
}

bool CConnSerSink::HandMainMsgNet(USHORT nIndex,USHORT nSub, void* pData, USHORT nDataSize)
{
	switch (nSub)
	{
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

bool CConnSerSink::HandMainMsgToRoom(USHORT nIndex,USHORT nSub, void* pData, USHORT nDataSize)
{
	USHORT nGid, nGsid,nSeatNo;
	m_pUserInfo->GetGameInfo(nGid, nGsid,nSeatNo);
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
	memcpy(pUserInfo,m_pUserInfo->GetUserBaseInfo(),sizeof(UserBaseInfo));
	
	USHORT nSerNo = 0;
	if(SUB_MSG_CREATE == nSub)
	{
		CreateRoom* pCreate = (CreateRoom*)pData;
		nSerNo = g_pConnectServer->GetGameSerNoByGameId(pCreate->roominfo.nGameId);
	}
	else if(SUB_MSG_ENTER == nSub)
	{
		int nRoomId = *(int*)pData;
		nSerNo = nRoomId / 100000;
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
	
	SendToGameSer(nSerNo, MAIN_MSG_CONNSER, SUB_MSG_USER4ROOM, buff, nDataSize + sizeof(User2Room) + sizeof(UserBaseInfo));
	return true;
}

bool CConnSerSink::HandMainMsgToGame(USHORT nIndex,USHORT nMain,USHORT nSub, void* pData, USHORT nDataSize)
{
	USHORT nGameSerNo,nGameSerIndex,nGameSerSeatNo;
	m_pUserInfo->GetGameInfo(nGameSerNo,nGameSerIndex,nGameSerSeatNo);
	char buff[MAX_MSG_SIZE] = { 0 };
	User2Game* pCom = (User2Game*)buff;
	pCom->nIndex = nGameSerIndex;
	pCom->nSeatNo = nGameSerSeatNo;
	pCom->nMain = nMain;
	pCom->nSub = nSub;
	memcpy(pCom + 1, pData, nDataSize);
	SendToGameSer(nGameSerNo, MAIN_MSG_CONNSER, SUB_MSG_USER4GAME, buff, nDataSize + sizeof(User2Game));
	return true;
}

bool CConnSerSink::HandMainMsgFromConnect(USHORT nIndex,USHORT nSub, void* pData, USHORT nDataSize)
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

