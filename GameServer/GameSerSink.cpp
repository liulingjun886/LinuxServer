#include "GameSerSink.h"
#include "../commproto.h"
#include "GameServer.h"
#include "../include/Services.h"
#include "../NetSinkObj.h"
#include "GameUserEnginer.h"

extern CGameServer* g_pGameServer;

enum TIME_ID
{
	TIME_TEST_LINK = 1,
};


CGameSerSink::CGameSerSink(CServices* pServices):CNetHandSink(pServices)
{
	m_timer_Link.InitTimerObj(m_pNet, TIME_TEST_LINK);
}

CGameSerSink::~CGameSerSink()
{
	
}

void CGameSerSink::Connect()
{
	COutputPacket out;
	out.Begin(MAIN_MSG_GAMESER, GS_SUB_MSG_CONN_SUCSS);
	out.WriteInt16(g_pGameServer->GetSerType());
	out.WriteInt16(g_pGameServer->GetSerNo());
	out.End();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), out);
}

void CGameSerSink::Close()
{
	g_pGameServer->DelConnSrvIndex(m_nConnNo, m_pNet->GetServiceIndex());
}

bool CGameSerSink::HandTimeMsg(uint16 nTimeID)
{
	switch(nTimeID)
	{
		case TIME_TEST_LINK:
		{
			++m_nTestNum;
			if(m_nTestNum > 1)
			{
				m_pNet->Log("test link timeout!");
				return false;
			}
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);
			return true;
		}
	}
	return false;
}


bool CGameSerSink::HandNetData(uint16 nMain, uint16 nSub, CInputPacket& inPacket)
{
	switch(nMain)
	{
		case MAIN_MSG_CONNSER:
			return HandMsgFromConnSrv(nSub, inPacket);
		default:
			break;
	}
	return false;
}

bool CGameSerSink::HandTestNetConn()
{
	m_nTestNum = 0;
	COutputPacket out;
	out.Begin(MAIN_MSG_GAMESER, GS_SUB_MSG_TEST);
	out.End();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), out);
	return true;
}


bool CGameSerSink::HandMsgFromConnSrv(uint16 nSub, CInputPacket& inPacket)
{
	switch(nSub)
	{
		case CS_SUB_MSG_REG_CONN:
		{
			uint16 nSrvNo = inPacket.ReadInt16();
			g_pGameServer->AddConnInfo(nSrvNo, m_pNet->GetServiceIndex());
			HandTestNetConn();
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);
			break;
		}
		case CS_SUB_MSG_TEST:
		{
			HandTestNetConn();
			break;
		}
		case CS_SUB_MSG_USER4HALL:
		{
			HandMsgFromUserForHall(inPacket);
			break;
		}
		case CS_SUB_MSG_USER4ROOM:
		{
			HandMsgFromUserForRoom(inPacket);
			break;
		}
		case CS_SUB_MSG_USER2ROOM:
		{
			HandMsgFromUserToGame(inPacket);
			break;
		}
		case CS_SUB_MSG_USER:
		{
			HandMsgFromUser(inPacket);
			break;
		}
		default:
			break;
	}
	return true;
}


void CGameSerSink::HandMsgFromUserToGame(CInputPacket& inPacket)
{
	SERVICEINDEX nIndex = inPacket.ReadInt16();
	m_pNet->PostData(nIndex, USER_NET_MSG, (void*)inPacket.RestPacket(), inPacket.Rest_Len());
}

void CGameSerSink::HandMsgFromUserForRoom(CInputPacket& inPacket)
{
	UID nUserId = inPacket.ReadInt32();
	SERVICEINDEX nIndex = Single_Get(CGameUserEnginer)->GetUserManagerIndex(nUserId);
	m_pNet->PostData(nIndex, USER_NET_MSG, inPacket.RestPacket(), inPacket.Rest_Len());
	//m_pNet->PostData(nIndex, USER_NET_MSG, (void*)inPacket.RestPacket(), inPacket.Rest_Len());
}

void CGameSerSink::HandMsgFromUserForHall(CInputPacket& inPacket)
{
	UID nUserId = inPacket.ReadInt32();

	SERVICEINDEX nIndex = Single_Get(CGameUserEnginer)->GetUserManagerIndex(nUserId);
	m_pNet->PostData(nIndex, USER_NET_MSG, inPacket.RestPacket(), inPacket.Rest_Len());
}

void CGameSerSink::HandMsgFromUser(CInputPacket& inPacket)
{
	UID nUserId = inPacket.ReadInt32();
	SERVICEINDEX nIndex = inPacket.ReadInt16();

	if(INVALID_SERIVCE_INDEX == nIndex)
		nIndex = Single_Get(CGameUserEnginer)->GetUserManagerIndex(nUserId);

	m_pNet->PostData(nIndex, USER_NET_MSG, inPacket.RestPacket(), inPacket.Rest_Len());
}

