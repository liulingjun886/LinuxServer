#include "UserCliSink.h"
#include "../NetSinkObj.h"
#include "../include/Services.h"
#include "../commproto.h"

extern CUserServer* g_pUserServer;


enum TIME_ID
{
	TIME_TEST_CONN=1,
	TIME_RECONNECT,
};

CUserCliSink::CUserCliSink(CServices* pServices):CNetHandSink(pServices)
	,m_nTestNum(0)
	,m_nReConnectCount(0)
{
	m_timer_Link.InitTimerObj(m_pNet, TIME_TEST_CONN);
	m_timer_reconnect.InitTimerObj(m_pNet, TIME_RECONNECT);
}

CUserCliSink::~CUserCliSink()
{
	
}

bool CUserCliSink::HandTimeMsg(uint16 nTimeID)
{
	switch(nTimeID)
	{
		case TIME_TEST_CONN:
		{
			++m_nTestNum;
			if(m_nTestNum > 1)
			{
				m_pNet->Log("test link times = %d", m_nTestNum);
				return false;
			}
			
			m_timer_Link.StartTimerSec(CLIENT_TEST_TIME);
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_USERSER, US_SUB_MSG_TEST);
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
	return true;
}

bool CUserCliSink::HandNetData(uint16 nSrcIndex, uint16 nMain, uint16 nSub, void* pData, uint32 nDataSize)
{
	m_pNet->Log("Recv cmd %d, %d", nMain, nSub);
	switch(nMain)
	{
		case MAIN_MSG_CENTERSER:
		{
			return HandMainMsgFromCenter(nSrcIndex, nSub, pData, nDataSize);
		}
		default:
			m_pNet->Log("invalid cmd main=%d,sub=%d", nMain,nSub);
	}
	return true;
}

bool CUserCliSink::DisConnect()
{
	m_timer_Link.StopTimer();
	m_timer_reconnect.StopTimer();
	m_timer_reconnect.StartTimerSec(CLIENT_RECONN_TIME);
	return true;
}


bool CUserCliSink::HandMainMsgFromCenter(uint16 nSrcIndex, uint16 nSub, void* pData, uint32 nDataSize)
{
	switch(nSub)
	{
		case CT_SUB_MSG_CONN_SUCSS:
		{
			m_nTestNum = 0;
			m_nReConnectCount = 0;
			m_timer_Link.StartTimerSec(CLIENT_TEST_TIME);
			RegConnSer ser;
			ser.nSerNo = g_pUserServer->GetSerNo();
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_USERSER, US_SUB_MSG_REGUSERSRV, &ser, sizeof(ser));
			return true;
		}
		case CT_SUB_MSG_TEST:
		{
			m_nTestNum = 0;
			return true;
		}
	}
	return true;
}

