#include "UserSerSink.h"
#include "../NetSinkObj.h"
#include "UserServer.h"
#include "../include/Services.h"
#include "../commproto.h"

extern CUserServer* g_pUserServer;

enum TIME_ID
{
	TIME_TEST_LINK=1,
};


CUserSerSink::CUserSerSink(CServices* pServices):CNetHandSink(pServices)
{
	m_timer_Link.InitTimerObj(m_pNet, TIME_TEST_LINK);
}

CUserSerSink::~CUserSerSink()
{
	
}

bool CUserSerSink::HandTimeMsg(uint16 nTimeID)
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

bool CUserSerSink::HandNetData(uint16 nMain, uint16 nSub, CInputPacket& inPacket)
{
	m_pNet->Log("Recv cmd %d, %d", nMain, nSub);
	switch(nMain)
	{
		case MAIN_MSG_GAMESER:
			return HandMainMSgGameSer(nSub, inPacket);
		case MAIN_MSG_CONNSER:
			return HandMainMsgConnSer(nSub, inPacket);
		default:
			break;
	}
	return false;
}


bool CUserSerSink::HandTestNetConn()
{
	m_nTestNum = 0;
	COutputPacket out;
	out.Begin(MAIN_MSG_USERSER, US_SUB_MSG_TEST);
	out.End();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), out);
	return true;
}


void CUserSerSink::Connect()
{
	COutputPacket out;
	out.Begin(MAIN_MSG_USERSER, US_SUB_MSG_CONN_SUCSS);
	out.WriteInt16( g_pUserServer->GetSerType());
	out.WriteInt16( g_pUserServer->GetSerNo());
	out.End();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), out);
}

bool CUserSerSink::HandMainMSgGameSer(uint16 nSub, CInputPacket& inPacket)
{
	switch(nSub)
	{
		case GS_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
		case GS_SUB_MSG_REG_GAMESRV:
		{
			HandTestNetConn();
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);
			return true;
		}
	}
	return true;
}

bool CUserSerSink::HandMainMsgConnSer(uint16 nSub, CInputPacket& inPacket)
{
	switch(nSub)
	{
		case CS_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
		case CS_SUB_MSG_REG_CONN:
		{
			HandTestNetConn();
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);
			return true;
		}
		case CS_SUB_MSG_USER_LOGIN_HALL:
		{
			
			//g_pUserServer->PostMemDataBaseReq(m_pNet, void * pData, DATASIZE uDataSize)
			break;
		}
	}
	return true;
}

