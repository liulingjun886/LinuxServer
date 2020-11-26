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
{
	m_timer_Link.InitTimerObj(m_pNet, TIME_TEST_CONN);
	m_timer_reconnect.InitTimerObj(m_pNet, TIME_RECONNECT);
}

CUserCliSink::~CUserCliSink()
{
	
}

bool CUserCliSink::HandTimeMsg(USHORT nTimeID)
{
	switch(nTimeID)
	{
		case TIME_TEST_CONN:
		{
			++m_nTestNum;
			if(m_nTestNum > 1)
				return false;
			
			m_timer_Link.StartTimerSec(30);
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_USERSER, SUB_MSG_TEST);
			return true;
		}
		case TIME_RECONNECT:
		{
			++m_nReConnectCount;
			if(m_nReConnectCount == 3)
			{
				return false;
			}
			m_pNet->PostData(m_pNet->GetServiceIndex(), NET_RECONNECT);
			return true;
		}
	}
	return true;
}

bool CUserCliSink::HandNetData(USHORT nSrcIndex, USHORT nMain, USHORT nSub, void* pData, UINT nDataSize)
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
	m_timer_reconnect.StopTimer();
	m_timer_reconnect.StartTimerSec(30);
	return true;
}


bool CUserCliSink::HandMainMsgFromCenter(USHORT nSrcIndex, USHORT nSub, void* pData, UINT nDataSize)
{
	switch(nSub)
	{
		case CT_SUB_MSG_CONN_SUCSS:
		{
			m_timer_Link.StartTimerSec(30);
			RegConnSer ser;
			ser.nSerNo = g_pUserServer->GetSerNo();
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_USERSER, US_SUB_MSG_REGUSERSRV, &ser, sizeof(ser));
			m_pNet->Log("Recv CT_SUB_MSG_CONN_SUCSS");
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

