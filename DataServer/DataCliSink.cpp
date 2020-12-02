#include "DataCliSink.h"
#include "../Defines.h"
#include "../NetSinkObj.h"
#include "../include/Services.h"
#include "../commproto.h"

extern CDataServer* g_pDataServer;

enum TIME_ID
{
	TIME_TEST_CONN=1,
	TIME_RECONNECT,
};


CDataCliSink::CDataCliSink(CServices* pNetSer):CNetHandSink(pNetSer)
{
	m_timer_Link.InitTimerObj(m_pNet, TIME_TEST_CONN);
	m_timer_reconnect.InitTimerObj(m_pNet, TIME_RECONNECT);
}

CDataCliSink::~CDataCliSink()
{
	
}


bool CDataCliSink::HandTimeMsg(uint16 nTimeID)
{
	switch(nTimeID)
	{
		case TIME_TEST_CONN:
		{
			++m_nTestNum;
			if(m_nTestNum > 1)
				return false;
			
			m_timer_Link.StartTimerSec(CLIENT_TEST_TIME);
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_DATASER, DS_SUB_MSG_TEST);
			return true;
		}
		case TIME_RECONNECT:
		{
			++m_nReConnectCount;
			if(m_nReConnectCount >= CLIENT_RECONN_NUMS)
			{
				m_pNet->Log("m_nReConnectCount = %d", m_nReConnectCount);
				return false;
			}
			m_pNet->PostData(m_pNet->GetServiceIndex(), NET_RECONNECT);
			return true;
		}
	}
	return false;
}

void CDataCliSink::SendMsgToCenterSrv(COutputPacket& out)
{
	CNetSinkObj::SendData(m_pNet, g_pDataServer->GetCenterIndex(), out);
}


bool CDataCliSink::HandNetData(uint16 nMain, uint16 nSub, CInputPacket& inPacket)
{
	m_pNet->Log("CLient Recv cmd %d, %d", nMain, nSub);
	switch(nMain)
	{
		case MAIN_MSG_CENTERSER:
		{
			return HandMainMsgFromCenterSrv(nSub, inPacket);
		}
		default:
			break;
	}
	return false;
}

bool CDataCliSink::DisConnect()
{
	m_timer_Link.StopTimer();
	m_timer_reconnect.StartTimerSec(CLIENT_RECONN_TIME);
	return true;
}

void CDataCliSink::RegDataSrv()
{
	COutputPacket out;
	out.Begin(MAIN_MSG_DATASER, DS_SUB_MSG_REG_DATASRV);
	out.WriteInt16(g_pDataServer->GetSerNo());
	out.End();
	SendMsgToCenterSrv(out);
}


bool CDataCliSink::HandMainMsgFromCenterSrv(uint16 nSub, CInputPacket& inPacket)
{
	switch(nSub)
	{
		case CT_SUB_MSG_TEST:
		{
			m_nTestNum = 0;
			return true;
		}
		case CT_SUB_MSG_CONN_SUCSS:
		{
			m_nTestNum = 0;
			m_nReConnectCount = 0;
			m_timer_reconnect.StopTimer();
			m_timer_Link.StartTimerSec(CLIENT_TEST_TIME);
			RegDataSrv();
			return true;
		}
		default:
			break;
	}
	return false;
}

