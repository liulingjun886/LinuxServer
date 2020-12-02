#include "ConnCliSink.h"
#include <stdio.h>
#include "../commproto.h"
#include "../include/Core.h"
#include "../UserServer/MemDataDef.h"
#include "../NetSinkObj.h"
#include "../include/Services.h"
#include "ConnectServer.h"

extern CConnectServer* g_pConnectServer;

enum TIME_ID
{
	TIME_CONN_IS_LINK = 1,
	TIME_CONN_RECONNECT,
};

CConnCliSink::CConnCliSink(CServices* pNet) :CNetHandSink(pNet),m_nReConnectCount(0)
{
	m_timer_Link.InitTimerObj(m_pNet, TIME_CONN_IS_LINK);
	m_timer_reconnect.InitTimerObj(m_pNet, TIME_CONN_RECONNECT);
}


CConnCliSink::~CConnCliSink()
{
	
}

void CConnCliSink::Init(const char* szIp)
{

}

bool CConnCliSink::DisConnect()
{	
	if(SRV_TYPE_GAME == m_nPeerSerType)
		return false;
	
	m_timer_Link.StopTimer();
	m_timer_reconnect.StartTimerSec(CLIENT_RECONN_TIME);
	return true;
}

void CConnCliSink::Close()
{
	g_pConnectServer->DisconnectToServer(m_nPeerSerType, m_nPeerSerNo, m_pNet->GetServiceIndex());
}


bool CConnCliSink::HandNetData(uint16 nMain, uint16 nSub, CInputPacket& inPacket)
{
	m_pNet->Log("Recv cmd %d, %d",nMain, nSub);
	switch (nMain)
	{
		case MAIN_MSG_CENTERSER:
		{
			return HandMsgFromCenterSrv(nSub, CInputPacket& inPacket);
		}
		case MAIN_MSG_USERSER:
		{
			return HandMsgFromUserSrv(nSub, CInputPacket& inPacket);		
		}
		case MAIN_MSG_GAMESER:
		{
			return HandMsgFromGameSrv(nSub,  CInputPacket& inPacket);
		}
		default:
			return false;
	}
	return false;
}

bool CConnCliSink::HandTimeMsg(uint16 uTimeID)
{
	switch (uTimeID)
	{
	case TIME_CONN_IS_LINK:
	{
		++m_nTestNum;
		if(m_nTestNum > 1)
			return false;
		
		m_timer_Link.StartTimerSec(CLIENT_TEST_TIME);
		CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CONNSER, CS_SUB_MSG_TEST);
		return true;
	}
	case TIME_CONN_RECONNECT:
	{
		++m_nReConnectCount;
		if(m_nReConnectCount >= CLIENT_RECONN_NUMS)
		{
			return false;
		}
		m_pNet->PostData(m_pNet->GetServiceIndex(), NET_RECONNECT);
		break;
	}
	default:
		break;
	}
	return true;
}

void CConnCliSink::SendData(uint16 nIndex, uint16 nMain, uint16 nSub, void* pData, DATASIZE nDataSize)
{
	CNetSinkObj::SendData(m_pNet, nIndex, nMain, nSub, pData, nDataSize);
}

void CConnCliSink::RegConnSrv()
{
	RegConnSer ser;
	ser.nSerNo = g_pConnectServer->GetSerNo();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CONNSER, CS_SUB_MSG_REG_CONN, &ser, sizeof(ser));
}

void CConnCliSink::ConnectSucess(CInputPacket& inPacket)
{
	m_nReConnectCount = 0;
	m_nPeerSerType = inPacket.ReadInt16();
	m_nPeerSerNo = inPacket.ReadInt16();
	m_timer_Link.StartTimerSec(CLIENT_TEST_TIME);
}


bool CConnCliSink::HandMsgFromCenterSrv(uint16 nSub, CInputPacket& inPacket)
{
	switch(nSub)
	{
		case CT_SUB_MSG_CONN_SUCSS:
		{
			ConnectSucess(inPacket);
			RegConnSrv();
			return true;
		}
		case CT_SUB_MSG_TEST:
		{
			m_nTestNum = 0;
			return true;
		}
		case CT_SUB_MSG_NEWGAMESER:
		{
			uint16 nNum = inPacket.ReadInt16();
			for(int i = 0; i < nNum; i++)
			{
				GameInfo info;
				info.szIp = inPacket.ReadString();
				info.nPort = inPacket.ReadInt16();
				info.nGameId = inPacket.ReadInt32();
				info.nGameNo = inPacket.ReadInt16();
				g_pConnectServer->ConnectToGameServer(info);
			}
			return true;
		}
			
	}
	return true;
}

bool CConnCliSink::HandMsgFromGameSrv(uint16 nSub, CInputPacket& inPacket)
{
	switch (nSub)
	{
		case GS_SUB_MSG_CONN_SUCSS:
		{
			ConnectSucess(inPacket);
			RegConnSrv();
			return true;
		}
		case GS_SUB_MSG_TEST:
		{
			m_nTestNum = 0;
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

bool CConnCliSink::HandMsgFromUserSrv(uint16 nSub, CInputPacket& inPacket)
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
			m_nTestNum = 0;
			return true;
		}
		default:
			break;
	}
	return true;
}

