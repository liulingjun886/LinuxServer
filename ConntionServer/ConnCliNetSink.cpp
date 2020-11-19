#include "ConnCliNetSink.h"
#include <stdio.h>
#include "commproto.h"
#include "Core.h"
#include "MemDataDef.h"
#include "NetSinkObj.h"
#include "Services.h"

extern USHORT g_serno;
extern UINT g_gameid;

CConnCliNetSink::CConnCliNetSink(CServices* pNet) :CCliNetSink(pNet),m_nReConnectCount(0),m_HandFun(NULL)
{
	m_timerConnTest.InitTimerObj(m_pNet, TIME_CONN_IS_LINK);
	m_timerReConn.InitTimerObj(m_pNet, TIME_CONN_RECONNECT);
}


CConnCliNetSink::~CConnCliNetSink()
{
	
}

void CConnCliNetSink::Init(UINT nIp)
{
}

bool CConnCliNetSink::DisConnect()
{	
	Single_Get(CCore)->Stop();
	DelDataSer();
	m_timerConnTest.StopTimer();
	m_timerReConn.StartTimerSec(5);
	return true;
}

bool CConnCliNetSink::HandNetData(USHORT nIndex,USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	//return (this->*m_HandFun)(nIndex,nMain,nSub,pData,nDataSize);
	m_nTestLink = 0;
	
	
	switch (nMain)
	{
	case MAIN_MSG_CENTERSER:
	{
		switch(nSub)
		{
			case CEN_SUB_MSG_CONN_SUCSS:
			{
				m_HandFun = &CConnCliNetSink::HandMsgFromCenter;
				return true;
			}
			default:
				break;
		}
		break;
	} 
	case MAIN_MSG_CONNECT:
	{
		break;
	}
	case MAIN_MSG_DATACENTER:
	{
		DataCenter* pCenter = (DataCenter*)(pData);
		if(SUB_MSG_DATA_BASE_RET == nSub)
		{
			m_pNet->PostData(pCenter->nCsid, DATA_BASE_RET, pCenter+1, nDataSize-sizeof(DataCenter));
		}
		else
		{
			m_pNet->PostData(pCenter->nCsid, MEM_DATA_BASE_RET, pCenter+1, nDataSize-sizeof(DataCenter));
		}
		break;
	}
	default:
		break;
	}
	if(m_HandFun)
		return (this->*m_HandFun)();
	return false;
}

bool CConnCliNetSink::HandTimeMsg(USHORT uTimeID)
{
	switch (uTimeID)
	{
	case TIME_CONN_IS_LINK:
	{
		CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_TEST);
		m_timerConnTest.StartTimerSec(5);
		break;
	}
	case TIME_CONN_RECONNECT:
	{
		++m_nReConnectCount;
		if(m_nReConnectCount == 5)
		{
			return false;
		}
		m_pNet->PostData(m_pNet->GetServiceIndex(), NET_RECONNECT);
		m_timerReConn.StartTimerSec(5);
		break;
	}
	default:
		break;
	}
	return true;
}

bool CConnCliNetSink::HandMainMsgNet(USHORT nIndex, USHORT nSub, void* pData, USHORT nDataSize)
{
	switch (nSub)
	{
		case SUB_MSG_DATA_SUCSS:
		{
			AddDataSer();
			return true;
		}
		case SUB_MSG_TEST:
		{
			CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_TEST);
			return true;
		}
		default:
			break;
	}
	return true;
}
