#include "ConnCliSink.h"
#include <stdio.h>
#include "../commproto.h"
#include "../include/Core.h"
#include "../MemDataBaseEnginer/MemDataDef.h"
#include "../NetSinkObj.h"
#include "../include/Services.h"
#include "ConnectServer.h"

extern CConnectServer* g_pConnectServer;


CConnCliSink::CConnCliSink(CServices* pNet) :CNetHandSink(pNet),m_nReConnectCount(0)
{
	m_timerConnTest.InitTimerObj(m_pNet, TIME_CONN_IS_LINK);
	m_timerReConn.InitTimerObj(m_pNet, TIME_CONN_RECONNECT);
}


CConnCliSink::~CConnCliSink()
{
	
}

void CConnCliSink::Init(UINT nIp)
{
}

bool CConnCliSink::DisConnect()
{	
	m_timerConnTest.StopTimer();
	m_timerReConn.StartTimerSec(30);
	return true;
}

void CConnCliSink::Close()
{
	g_pConnectServer->DisconnectToServer(m_nPeerSerType, m_nPeerSerNo, m_pNet->GetServiceIndex());
}


bool CConnCliSink::HandNetData(USHORT nIndex,USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	m_nTestLink = 0;
	switch (nMain)
	{
	case MAIN_MSG_CENTERSER:
	{
		return HandMsgFromCenterSrv(nIndex, nSub, pData, nDataSize);
	}
	case MAIN_MSG_USERSER:
	{
		return HandMsgFromUserSrv(nIndex, nSub, pData, nDataSize);		
	}
	case MAIN_MSG_GAMESER:
	{
		return HandMsgFromGameSrv(nIndex, nSub,  pData, nDataSize);
	}
	default:
		return false;
	}
	return false;
}

bool CConnCliSink::HandTimeMsg(USHORT uTimeID)
{
	switch (uTimeID)
	{
	case TIME_CONN_IS_LINK:
	{
		CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_CONNSER, SUB_MSG_TEST);
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
		//m_timerReConn.StartTimerSec(30);
		break;
	}
	default:
		break;
	}
	return true;
}

void CConnCliSink::SendData(USHORT nIndex, USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	CNetSinkObj::SendData(m_pNet, nIndex, nMain, nSub, pData, nDataSize);
}

void CConnCliSink::RegConnSrv()
{
	RegConnSer ser;
	ser.nSerNo = g_pConnectServer->GetSerNo();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CONNSER, CS_SUB_MSG_REG_CONN, &ser, sizeof(ser));
}

void CConnCliSink::ConnectSucess(ConnSucess* pConn)
{
	m_nReConnectCount = 0;
	m_nPeerSerType = pConn->nSrvType;
	m_nPeerSerNo = pConn->nSrvNo;
}


bool CConnCliSink::HandMsgFromCenterSrv(USHORT nSrcIndex, USHORT nSub, void* pData, USHORT nDataSize)
{
	switch(nSub)
	{
		case CT_SUB_MSG_CONN_SUCSS:
		{
			ConnSucess* pConn = (ConnSucess*)pData;
			ConnectSucess(pConn);
			RegConnSrv();
			return true;
		}
		case CT_SUB_MSG_TEST:
		{
			m_nTestLink = 0;
			return true;
		}
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
			return true;
		}
			
	}
	return true;
}

bool CConnCliSink::HandMsgFromGameSrv(USHORT nSrcIndex, USHORT nSub, void* pData, USHORT nDataSize)
{
	switch (nSub)
	{
		case GS_SUB_MSG_CONN_SUCSS:
		{
			ConnSucess* pConn = (ConnSucess*)pData;
			ConnectSucess(pConn);
			RegConnSrv();
			return true;
		}
		case GS_SUB_MSG_TEST:
		{
			m_nTestLink = 0;
			return true;
		}
		case GS_SUB_MSG_GAME2USER:
		{
			Game2User* pBuff = (Game2User*)pData;
			SendData(pBuff->nIndex, pBuff->nMain, pBuff->nSub, pBuff+1, nDataSize - sizeof(Game2User));
			return true;
		}
		case GS_SUB_MSG_GAME2CONN:
		{
			InnserSync* pSync = (InnserSync*)pData;
			m_pNet->PostData(pSync->nIndex, pSync->nType, pSync+1, nDataSize - sizeof(InnserSync));
			return true;
		}
		default:
			break;
	}
	return true;
}

bool CConnCliSink::HandMsgFromUserSrv(USHORT nSrcIndex, USHORT nSub, void* pData, USHORT nDataSize)
{
	switch(nSub)
	{
		case US_SUB_MSG_CONN_SUCSS:
		{
			ConnSucess* pConn = (ConnSucess*)pData;
			ConnectSucess(pConn);
			return true;
		}
		case US_SUB_MSG_TEST:
		{
			m_nTestLink = 0;
			return true;
		}
		default:
			break;
	}
	return true;
}

