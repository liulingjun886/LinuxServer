#include <stdio.h>
#include "DataSerSink.h"
#include "../include/core/Core.h"
#include "../include/core/ToolLock.h"
#include "../commproto.h"
#include "../include/core/Services.h"
#include "DataBaseDef.h"
#include "../NetSinkObj.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../DataBaseEnginer/DataBaseEnginer.h"
#include "../MemDataBaseEnginer/MemDataBaseEnger.h"
#include "DataServer.h"

extern CDataServer* g_pDataServer;

enum TIME_ID
{
	TIME_TEST_LINK = 1,
};

CDataSerSink::CDataSerSink(CServices* pNet) :CNetHandSink(pNet),m_nTestNum(0)
{
	m_timer_Link.InitTimerObj(m_pNet, TIME_TEST_LINK);
}


CDataSerSink::~CDataSerSink()
{
	
}

void CDataSerSink::Connect()
{
	COutputPacket out;
	out.Begin(MAIN_MSG_DATASER, DS_SUB_MSG_CONN_SUCSS);
	out.WriteInt16(g_pDataServer->GetSerType());
	out.WriteInt16(g_pDataServer->GetSerNo());
	out.End();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), out);
}

bool CDataSerSink::HandNetData(uint16 nMain, uint16 nSub, CInputPacket& inPacket)
{
	switch (nMain)
	{
	case MAIN_MSG_GAMESER:
	{
		return HandMainMsgFromGameSer(nSub,inPacket);
	}
	default:
		break;
	}
	return false;
}

bool CDataSerSink::HandTimeMsg(uint16 uTimeId)
{
	switch (uTimeId)
	{
		case TIME_TEST_LINK:
		{
			return TestNetLink();
		}
		default:
			break;
	}
	return true;
}

bool CDataSerSink::TestNetLink()
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

bool CDataSerSink::HandMainMsgFromGameSer(uint16 nSub, CInputPacket& inPacket)
{
	switch (nSub)
	{
		case GS_SUB_MSG_TEST:
		{
			HandTestNetConn();
			return true;
		}
		case GS_SUB_MSG_REG_GAMESRV:
		{
			HandTestNetConn();
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);
			return true;
		}
		default:
			return false;
	}
	return true;
}

bool CDataSerSink::HandTestNetConn()
{
	m_nTestNum = 0;
	COutputPacket out;
	out.Begin(MAIN_MSG_DATASER,DS_SUB_MSG_TEST);
	out.End();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), out);
	return true;
}


