#include <stdio.h>
#include "DataSerSink.h"
#include "../include/Core.h"
#include "../include/ToolLock.h"
#include "../commproto.h"
#include "../include/Services.h"
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
	ConnSucess conn;
	conn.nSrvNo = g_pDataServer->GetSerNo();
	conn.nSrvType = g_pDataServer->GetSerType();
	CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_DATASER, DS_SUB_MSG_CONN_SUCSS,&conn,sizeof(conn));
}

bool CDataSerSink::HandNetData(uint16 nIndex,uint16 nMain, uint16 nSub, void* pData, DATASIZE nDataSize)
{
	switch (nMain)
	{
	case MAIN_MSG_GAMESER:
	{
		return HandMainMsgFromGameSer(nIndex,nSub,pData,nDataSize);
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

bool CDataSerSink::HandMainMsgFromGameSer(uint16 nIndex,uint16 nSub,void* pData,DATASIZE nDataSize)
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
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_DATASER, DS_SUB_MSG_TEST);
	return true;
}


