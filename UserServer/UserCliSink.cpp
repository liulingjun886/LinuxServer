#include "UserCliSink.h"

extern CUserServer* g_pUserServer;


enum TIME_ID
{
	TIME_TEST_CONN=1,
};

CUserCliSink::CUserCliSink(CServices* pServices):CNetHandSink(pServices)
{
	m_timer_Link.InitTimerObj(m_pNet, TIME_TEST_CONN);
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
			
			m_timer_Link.StartTimerSec(300);
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_TEST);
			return true;
		}
	}
	return false;
}

bool CUserCliSink::HandNetData(USHORT nSrcIndex, USHORT nMain, USHORT nSub, void* pData, UINT nDataSize)
{
	switch(nMain)
	{
		case MAIN_MSG_CENTERSER:
		{
			return HandMainMsgCenter();
		}
		default:
			break;
	}
	return false;
}

bool CUserCliSink::HandMainMsgCenter(USHORT nSrcIndex, USHORT nSub, void* pData, UINT nDataSize)
{
	switch(nSub)
	{
		case SUB_MSG_CONN_SUCSS:
		{
			m_timer_Link.StartTimerSec(300);
			RegConnSer ser;
			ser.nSerNo = g_pUserServer->GetSerNo();
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_REG_USERSER, &ser, sizeof(ser));
			return true;
		}
		case SUB_MSG_TEST:
		{
			m_nTestNum = 0;
			return true;
		}
	}
	return false;
}

