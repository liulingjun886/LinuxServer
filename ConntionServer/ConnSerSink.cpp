#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/Core.h"
#include "../include/ToolLock.h"
#include "../commproto.h"
#include "../include/Services.h"
#include "../UserServer/MemDataDef.h"
#include "../DataServer/DataBaseDef.h"
#include "../NetSinkObj.h"
#include "ConnSerSink.h"
#include "ConnectServer.h"


extern CConnectServer* g_pConnectServer;

enum TIME_ID
{
	TIME_CONN_IS_LINK = 1,
};

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
	COutputPacket out;
	out.Begin(MAIN_MSG_CONNSER, CS_SUB_MSG_CONN_SUCSS);
	out.End();
	SendToMySelf(out);
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

	uint16 nGid,nGsid,nSeatNo;
	m_pUserInfo->GetGameInfo(nGid,nGsid,nSeatNo);
	
	if(nGid>0)
	{
		COutputPacket out;
		out.Begin(MAIN_MSG_CONNSER, SUB_MSG_USER4GAME);
		out.WriteInt16(nGsid);
		out.WriteInt16(MAIN_MSG_ROOM);
		out.WriteInt16(SUB_MSG_USER_DISCONNECT);
		out.WriteInt32(nSeatNo);
		out.End();
		
		SendToGameSer(nGid,out);
	}
	return false;
}


bool CConnSerSink::HandNetData(uint16 nMain, uint16 nSub, CInputPacket& inPacket)
{

	switch (nMain)
	{
//	case MAIN_MSG_NET:
//	{
//		return HandMainMsgNet(nIndex,nSub,pData,nDataSize);
//	}
//	case MAIN_MSG_LOGIN:
//	{
//		if(nDataSize != sizeof(DataBase::UserLoginReq))
//			return false;
//		if(NULL != m_pUserInfo)
//			return false;
//		DataBase::UserLoginReq* pReq = (DataBase::UserLoginReq*)pData;
//		CCliNetSink::PostDataBaseReq(m_pNet,DataBase::USER_LOGIN_REQ,pData,nDataSize);
//		return true;
//	}
//	case MAIN_MSG_ROOM_MANAGER:
//	{
//		return HandMainMsgToRoom(nIndex,nSub,pData,nDataSize);
//	}
	case MAIN_MSG_ROOM:
	case MAIN_MSG_GAME:
	{
		return HandMainMsgToGame(nMain,nSub,inPacket);
	}
//	case MAIN_MSG_CONNECT:
//	{
//		return HandMainMsgFromConnect(nIndex,nSub,pData,nDataSize);
//	}
//	case MAIN_ERROR:
//	{
//		return false;
//	}
	default:
		break;
	}
	return false;
}

bool CConnSerSink::HandTimeMsg(uint16 uTimeId)
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
	COutputPacket out;
	out.Begin(MAIN_MSG_CONNSER, CS_SUB_MSG_TEST);
	out.End();
	SendToMySelf(out);
	m_timerConnTest.StartTimerSec(60);
	return true;
}

bool CConnSerSink::SendToCenterSer(COutputPacket& out)
{
	CNetSinkObj::SendData(m_pNet, g_pConnectServer->GetCenterServerIndex(), out);
	return true;
}

bool CConnSerSink::SendToMySelf(COutputPacket& out)
{
	CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), out);
	return true;
}

bool CConnSerSink::SendToGameSer(uint16 nSerNo, COutputPacket& out)
{
	uint16 nIndex = g_pConnectServer->GetGameSerIndexByNo(nSerNo, m_pNet->GetServiceIndex());
	CNetSinkObj::SendData(m_pNet, nIndex, out);
	return true;
}

bool CConnSerSink::SendToUserSer(uint32 nUid, COutputPacket& out)
{
	uint16 nIndex = g_pConnectServer->GetUserServerIndex(nUid);
	CNetSinkObj::SendData(m_pNet, nIndex, out);
	return true;
}

bool CConnSerSink::SendToConnectSer(COutputPacket& out)
{
	uint16 nTempGameSerNo = g_pConnectServer->GetARandGameSer();
	return SendToGameSer(nTempGameSerNo, out);
}


bool CConnSerSink::HandDataBaseRet(uint32 nType, void * pData, DATASIZE nDataSize)
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
			break;
		}
		default:
			return false;
	}
	return true;
}

bool CConnSerSink::HandMemDataRet(uint32 nType, void* pData, DATASIZE uDataSize)
{
/*	switch (nType)
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
					//uint16 nIndex = g_pConnectServer->GetCenterServerIndex();
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
	}*/
	return true;
}

bool CConnSerSink::HandUserMsg(int nEvent, void * pData, DATASIZE nDataSize)
{

	switch (nEvent)
	{
		case DATA_BASE_RET:
		{
			uint32 *pType = (uint32*)pData;
			return HandDataBaseRet(*pType, pType+1,  nDataSize-sizeof(uint32));
		}
		case MEM_DATA_BASE_RET:
		{
			uint32 *pType = (uint32*)pData;
			return HandMemDataRet(*pType, pType+1,  nDataSize-sizeof(uint32));
		}
		case EN_DOUBLE_LOGIN:
		{
			UID *pUid = (UID*)(pData);
			if(m_pUserInfo && *pUid == m_pUserInfo->GetUserId())
			{
				COutputPacket out;
				out.Begin(MAIN_MSG_CONNSER, SUB_MSG_DOUBLE_LOGIN);
				out.End();
				SendToMySelf(out);
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

bool CConnSerSink::HandMainMsgNet(uint16 nSub, CInputPacket& inPacket)
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

bool CConnSerSink::HandMainMsgToRoom(uint16 nSub, CInputPacket& inPacket)
{
	uint16 nGid, nGsid,nSeatNo;
	m_pUserInfo->GetGameInfo(nGid, nGsid,nSeatNo);
	if(nGid>0)
	{
		return true;
	}

	COutputPacket out;
	out.Begin(MAIN_MSG_CONNSER, SUB_MSG_USER4ROOM);
	out.WriteInt16(m_pNet->GetServiceIndex());
	out.WriteInt16(MAIN_MSG_GAMESER);
	out.WriteInt16(nSub);
	out.WriteBinary(m_pUserInfo->GetUserBaseInfo(), (uint32)sizeof(UserBaseInfo));
	
	uint16 nSerNo = 0;
	if(SUB_MSG_CREATE == nSub)
	{
		CreateRoom* pCreate = (CreateRoom*)inPacket.ReadBinary((uint32)sizeof(CreateRoom));
		nSerNo = g_pConnectServer->GetGameSerNoByGameId(pCreate->roominfo.nGameId);
		out.WriteBinary(pCreate, (uint32)sizeof(CreateRoom));
	}
	else if(SUB_MSG_ENTER == nSub)
	{
		int nRoomId = inPacket.ReadInt32();
		nSerNo = nRoomId / 100000;
		out.WriteInt32(nRoomId);
	}
	else
	{
		return true;
	}
	
	if(nSerNo == INVALID_SERIVCE_INDEX)
		printf("nSerNo falt\n");

	out.End();
	
	SendToGameSer(nSerNo, out);
	return true;
}

bool CConnSerSink::HandMainMsgToGame(uint16 nMain,uint16 nSub, CInputPacket& inPacket)
{
	uint16 nGameSerNo,nGameSerIndex,nGameSerSeatNo;
	m_pUserInfo->GetGameInfo(nGameSerNo,nGameSerIndex,nGameSerSeatNo);

	COutputPacket out;
	out.Begin(MAIN_MSG_CONNSER,CS_SUB_MSG_USER2ROOM);
	out.WriteInt16(nGameSerIndex);
	out.WriteInt32(m_nUserId);
	out.WriteBinary(inPacket.Get_Packet(), inPacket.Packet_Len());
	out.End();
	SendToGameSer(nGameSerNo, out);
	return true;
}

bool CConnSerSink::HandMainMsgFromConnect(uint16 nSub, CInputPacket& inPacket)
{
	switch (nSub)
	{
		case SUB_MSG_USER_DOUBLELOGIN:
		{
			UserDoubleLogin* pDouble = (UserDoubleLogin*)inPacket.ReadBinary((uint32)sizeof(UserDoubleLogin));
			m_pNet->PostData(pDouble->nCsid,EN_DOUBLE_LOGIN,&pDouble->nUserId,sizeof(UID));
			break;
		}
		default:
			break;
	}
	return true;
}

