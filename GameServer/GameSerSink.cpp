#include "GameSerSink.h"
#include "../commproto.h"
#include "GameServer.h"
#include "../include/Services.h"
#include "../NetSinkObj.h"

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
	ConnSucess conn;
	conn.nSrvNo = g_pGameServer->GetSerNo();
	conn.nSrvType = g_pGameServer->GetSerType();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_GAMESER, GS_SUB_MSG_CONN_SUCSS, &conn, sizeof(conn));
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
			return true;
		}
		case CS_SUB_MSG_TEST:
		{
			HandTestNetConn();
			return true;
		}
		default:
			break;
	}
	return true;
}

