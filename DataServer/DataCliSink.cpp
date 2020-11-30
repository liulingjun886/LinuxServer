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

void CDataCliSink::SendMsgToCenterSrv(uint16 nMain, uint16 nSub, void* pData, DATASIZE nDataSize)
{
	CNetSinkObj::SendData(m_pNet,g_pDataServer->GetCenterIndex(), nMain, nSub, pData, nDataSize);
}


bool CDataCliSink::HandNetData(uint16 nSrcIndex, uint16 nMain, uint16 nSub, void* pData, uint32 nDataSize)
{
	m_pNet->Log("CLient Recv cmd %d, %d", nMain, nSub);
	switch(nMain)
	{
		case MAIN_MSG_CENTERSER:
		{
			return HandMainMsgFromCenterSrv(nSrcIndex, nSub, pData, nDataSize);
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


bool CDataCliSink::HandMainMsgFromCenterSrv(uint16 nSrcIndex, uint16 nSub, void* pData, uint32 nDataSize)
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
			RegConnSer ser;
			ser.nSerNo = g_pDataServer->GetSerNo();
			CNetSinkObj::SendData(m_pNet, nSrcIndex, MAIN_MSG_DATASER, DS_SUB_MSG_REG_DATASRV, &ser, sizeof(ser));
			return true;
		}
		default:
			break;
	}
	return false;
}

