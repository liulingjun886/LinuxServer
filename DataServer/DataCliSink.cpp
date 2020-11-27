#include "DataCliSink.h"
#include "../Defines.h"
#include "../NetSinkObj.h"
#include "../include/Services.h"
#include "../commproto.h"

extern CDataServer* g_pDataServer;

enum TIME_ID
{
	TIME_TEST_CONN=1,
};


CDataCliSink::CDataCliSink(CServices* pNetSer):CNetHandSink(pNetSer)
{
	m_timer_Link.InitTimerObj(m_pNet, 1);
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
			
			m_timer_Link.StartTimerSec(300);
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_DATASER, DS_SUB_MSG_TEST);
			return true;
		}
	}
	return false;
}

bool CDataCliSink::HandNetData(uint16 nSrcIndex, uint16 nMain, uint16 nSub, void* pData, uint32 nDataSize)
{
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
			m_timer_Link.StartTimerSec(300);
			RegConnSer ser;
			ser.nSerNo = g_pDataServer->GetSerNo();
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_DATASER, SUB_MSG_REG_DATASER, &ser, sizeof(ser));
			return true;
		}
		default:
			break;
	}
	return false;
}

