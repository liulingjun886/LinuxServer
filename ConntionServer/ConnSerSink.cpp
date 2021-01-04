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
	TIME_LOGIN,
};

CConnSerSink::CConnSerSink(CServices* pNet) :CNetHandSink(pNet),m_nTestLink(0)
{
	m_timerConnTest.InitTimerObj(m_pNet, TIME_CONN_IS_LINK);
	m_timer_Login.InitTimerObj(m_pNet, TIME_LOGIN);
}


CConnSerSink::~CConnSerSink()
{
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

	m_timer_Login.StartTimerSec(3);
}

bool CConnSerSink::DisConnect()
{

	
	return false;
}


bool CConnSerSink::HandNetData(uint16 nMain, uint16 nSub, CInputPacket& inPacket)
{

	switch (nMain)
	{
	case MAIN_MSG_HALL:
	{
		m_timer_Login.StopTimer();
		return HandMainMsgToHall(nMain,nSub,inPacket);
	}
	case MAIN_MSG_ROOM:
	case MAIN_MSG_GAME:
	{
		return HandMainMsgToGame(nMain,nSub,inPacket);
	}
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
	case TIME_LOGIN:
	{
		break;
	}
	default:
		break;
	}
	return false;
}

void CConnSerSink::Close()
{
	
	COutputPacket userleave;
	userleave.Begin(MAIN_MSG_CONNSER, CS_SUB_MSG_USER_CLOSE);
	userleave.WriteInt32(m_nUserId);
	userleave.WriteInt16(g_pConnectServer->GetSerNo());
	userleave.WriteInt16(m_pNet->GetServiceIndex());
	SendToUserSer(m_nUserId, userleave);
	
	COutputPacket offline;
	offline.Begin(MAIN_MSG_CONNSER, CS_SUB_MSG_USER_CLOSE);
	offline.WriteInt32(m_nUserId);
	offline.WriteInt16(g_pConnectServer->GetSerNo());
	offline.WriteInt16(m_pNet->GetServiceIndex());
	offline.End();
	SendToGameSer(m_nGameSrvNo,offline);
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


bool CConnSerSink::HandDataBaseRet(uint32 nType, CInputPacket& inPacket)
{
	switch (nType)
	{
		case DataBase::USER_LOGIN_RET:
		{
			DataBase::UserLoginRet* pRet = (DataBase::UserLoginRet*)inPacket.ReadBinary(inPacket.Rest_Len());
			
//			m_pUserInfo = new CUserInfo;
//			m_pUserInfo->SetUserBaseInfo(pRet->nUserId, pRet->nSex, pRet->szName, pRet->szHeadUrl);
//			m_pUserInfo->UpdateConnInfo(g_pConnectServer->GetSerNo(), m_pNet->GetServiceIndex());
//
//			Mem::UserLoginMemReq req;
//			req.nUserId = m_pUserInfo->GetUserId();
//			req.nSerNo = g_pConnectServer->GetSerNo();
//			req.nSockNo = m_pNet->GetServiceIndex();
			break;
		}
		default:
			return false;
	}
	return true;
}

bool CConnSerSink::HandMemDataRet(uint32 nType, CInputPacket& inPacket)
{
	switch (nType)
	{
		case Mem::USER_LOGIN_RET:
		{
			m_nGameSrvNo = inPacket.ReadInt16();
			m_nGameSrvIndex = inPacket.ReadInt16();

			if(m_nGameSrvIndex > 0)
			{
				COutputPacket out;
				out.Begin(MAIN_MSG_CONNSER,SUB_MSG_USER_RELOGIN);
				out.WriteInt32(m_nUserId);
				out.WriteInt16(g_pConnectServer->GetSerNo());
				out.WriteInt16(m_pNet->GetServiceIndex());
				out.End();
				SendToGameSer(m_nGameSrvNo, out);
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

bool CConnSerSink::HandUserMsg(int nEvent, void * pData, DATASIZE nDataSize)
{
	switch (nEvent)
	{
		case DATA_BASE_RET:
		{
			CInputPacket in;
			in.Copy(pData, nDataSize, false);
			uint32 nEvType = in.ReadInt32();
			return HandDataBaseRet(nEvType, in);
		}
		case MEM_DATA_BASE_RET:
		{
			CInputPacket in;
			in.Copy(pData, nDataSize, false);
			uint32 nEvType = in.ReadInt32();
			return HandMemDataRet(nEvType, in);
		}
		case EN_DOUBLE_LOGIN:
		{
			UID *pUid = (UID*)(pData);
//			if(m_pUserInfo && *pUid == m_pUserInfo->GetUserId())
//			{
//				COutputPacket out;
//				out.Begin(MAIN_MSG_CONNSER, SUB_MSG_DOUBLE_LOGIN);
//				out.End();
//				SendToMySelf(out);
//				m_pNet->PostData(m_pNet->GetServiceIndex(), EXIT_MSG);
//			}
			break;
		}
		case EN_SYNC_GAME_SER_INFO:
		{
			UserGameSerInfo* pInfo = (UserGameSerInfo*)(pData);
//			if(m_pUserInfo && m_pUserInfo->GetUserId() == pInfo->nUserId)
//			{
//				m_pUserInfo->UpdateGameInfo(pInfo->nGid, pInfo->nGsid, pInfo->nSeatNo);
//			}
			break;
		}
		case EN_RECONNECT_FAIL:
		{
//			Mem::UserQuitGameReq req;
//			req.nUserId = m_pUserInfo->GetUserId();
//			m_pUserInfo->UpdateGameInfo(0, 0, 0);
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
//	m_pUserInfo->GetGameInfo(nGid, nGsid,nSeatNo);
	if(nGid>0)
	{
		return true;
	}

	COutputPacket out;
	out.Begin(MAIN_MSG_CONNSER, SUB_MSG_USER4ROOM);
	out.WriteInt16(m_pNet->GetServiceIndex());
	out.WriteInt16(MAIN_MSG_GAMESER);
	out.WriteInt16(nSub);
//	out.WriteBinary(m_pUserInfo->GetUserBaseInfo(), (uint32)sizeof(UserBaseInfo));
	
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
	//m_pUserInfo->GetGameInfo(nGameSerNo,nGameSerIndex,nGameSerSeatNo);
	UID nUserId = inPacket.ReadInt32();
//	if(nUserId != m_pUserInfo->GetUserId())
//		return false;
	
	COutputPacket out;
	out.Begin(MAIN_MSG_CONNSER,CS_SUB_MSG_USER2ROOM);
	out.WriteInt16(nGameSerIndex);
	out.WriteBinary(inPacket.Get_Packet(), inPacket.Packet_Len());
	out.End();
	SendToGameSer(nGameSerNo, out);
	return true;
}

bool CConnSerSink::HandMainMsgToHall(uint16, uint16, CInputPacket& inPacket)
{
	UID nUserId = inPacket.ReadInt32();
	COutputPacket out;
	out.Begin(MAIN_MSG_CONNSER,CS_SUB_MSG_USER_LOGIN_HALL);
	out.WriteInt32(nUserId);
	out.WriteInt16(m_pNet->GetServiceIndex());
	out.End();
	SendToUserSer(g_pConnectServer->GetUserServerIndex(nUserId), out);
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

