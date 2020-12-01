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

bool CUserSerSink::HandNetData(uint16 nSrcIndex, uint16 nMain, uint16 nSub, void* pData, DATASIZE nDataSize)
{
	m_pNet->Log("Recv cmd %d, %d", nMain, nSub);
	switch(nMain)
	{
		case MAIN_MSG_GAMESER:
			return HandMainMSgGameSer(nSrcIndex,nSub,pData,nDataSize);
		case MAIN_MSG_CONNSER:
			return HandMainMsgConnSer(nSrcIndex,nSub,pData,nDataSize);
		default:
			break;
	}
	return false;
}

bool CUserSerSink::HandTestNetConn()
{
	m_nTestNum = 0;
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_USERSER, US_SUB_MSG_TEST);
	return true;
}


void CUserSerSink::Connect()
{
	ConnSucess conn;
	conn.nSrvNo = g_pUserServer->GetSerNo();
	conn.nSrvType = g_pUserServer->GetSerType();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_USERSER, US_SUB_MSG_CONN_SUCSS,&conn,sizeof(conn));
}

bool CUserSerSink::HandMainMSgGameSer(uint16 nSrcIndex, uint16 nSub, void* pData, DATASIZE nDataSize)
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

bool CUserSerSink::HandMainMsgConnSer(uint16 nSrcIndex, uint16 nSub, void* pData, DATASIZE nDataSize)
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
	}
	return true;
}

