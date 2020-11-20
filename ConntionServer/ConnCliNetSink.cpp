#include "ConnCliNetSink.h"
#include <stdio.h>
#include "commproto.h"
#include "Core.h"
#include "MemDataDef.h"
#include "NetSinkObj.h"
#include "Services.h"
#include "ConnectServer.h"

extern CConnectServer* g_pConnectServer;


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
	m_timerReConn.StartTimerSec(30);
	return true;
}

bool CConnCliNetSink::HandNetData(USHORT nIndex,USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	m_nTestLink = 0;
	if(m_HandFun)
		return (this->*m_HandFun)();
	
	switch (nMain)
	{
	case MAIN_MSG_CENTERSER:
	{
		if(CT_SUB_MSG_CONN_SUCSS == nSub)
		{
			m_HandFun = &CConnCliNetSink::HandMsgFromCenterSrv;
			RegConnSer ser;
			ser.nSerNo = g_pConnectServer->GetSerNo();
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CONNSER, CS_SUB_MSG_REG_CONN, &ser, sizeof(ser));
			return true;
		}
		return false;
	}
	case MAIN_MSG_USERSER:
	{
		if(US_SUB_MSG_CONN_SUCSS == nSub)
		{
			m_HandFun = &CConnCliNetSink::HandMsgFromUserSrv;
			return true;
		}
		return false;
	}
	case MAIN_MSG_GAMESER:
	{
		if(GS_SUB_MSG_CONN_SUCSS == nSub)
		{
			m_HandFun = &CConnCliNetSink::HandMsgFromGameSrv;
			return true;
		}
		return false;
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
		return false;
	}
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
		if(m_nReConnectCount == 3)
		{
			return false;
		}
		m_pNet->PostData(m_pNet->GetServiceIndex(), NET_RECONNECT);
		m_timerReConn.StartTimerSec(30);
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

bool CConnCliNetSink::HandMainMsgCenterSrv(USHORT nSrcIndex, USHORT nSub, void* pData, USHORT nDataSize)
{
	switch(nSub)
	{
		case CT_SUB_MSG_NEWGAMESER:
		{
			USHORT nNum = *(USHORT*)pData;
			if(nDataSize != sizeof(RegGameSer) * nNum + sizeof(USHORT))
				return true;
			RegGameSer* pInfo = (RegGameSer*)(((USHORT*)pData)+1);
			for(int i = 0; i < nNum; i++)
			{
				GameInfo info;
				info.szIp = pInfo->szIp;
				info.nPort = pInfo->nPort;
				info.nGameId = pInfo->nGameID;
				info.nGameNo = pInfo->nSerNo;
				g_pConnectServer->ConnectToGameServer(info);
			}
		}
		default:
			break;
	}
	return true;
}


bool CConnCliNetSink::HandMsgFromCenterSrv(USHORT nSrcIndex,USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	switch(nMain)
	{
		case MAIN_MSG_CENTERSER:
		{
			return HandMainMsgCenterSrv(nSrcIndex, nSub, pData, nDataSize);
		}
			
	}
	return false;
}

bool CConnCliNetSink::HandMsgFromGameSrv(USHORT nSrcIndex,USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	switch (nSub)
	{
		case GS_SUB_MSG_GAME4USER:
		{
			Game2User* pBuff = (Game2User*)pData;
			
			//if(pBuff->nMain == 6 && pBuff->nSub == 10)
			//{
				//static CMutexLock m;
				//static int nGameOver = 0;
				//CToolLock lock(&m);
				//m_pNet->Log("nGameOver = %d",++nGameOver);
				//printf("Recv Game Over Msg %d\n",m_pNet->GetServiceIndex());
			//}
			CNetSinkObj::SendData(m_pNet,pBuff->nIndex, pBuff->nMain, pBuff->nSub, pBuff+1, nDataSize-sizeof(Game2User));
			break;
		}
		case GS_SUB_MSG_GAME2CONN:
		{
			InnserSync* pSync = (InnserSync*)pData;
			m_pNet->PostData(pSync->nIndex, pSync->nType, pSync+1, nDataSize-sizeof(InnserSync));
			break;
		}
		default:
			break;
	}
	return true;
}

bool CConnCliNetSink::HandMsgFromUserSrv(USHORT nSrcIndex,USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	DataCenter* pCenter = (DataCenter*)(pData);
	if(US_SUB_MSG_DATA_BASE_RET == nSub)
	{
		m_pNet->PostData(pCenter->nCsid, DATA_BASE_RET, pCenter+1, nDataSize-sizeof(DataCenter));
	}
	else
	{
		m_pNet->PostData(pCenter->nCsid, MEM_DATA_BASE_RET, pCenter+1, nDataSize-sizeof(DataCenter));
	}
	return true;
}

