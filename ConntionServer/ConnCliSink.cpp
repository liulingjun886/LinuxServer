#include "ConnCliSink.h"
#include <stdio.h>
#include "../include/core/Core.h"
#include "../include/core/Services.h"
#include "../commproto.h"
#include "../UserServer/MemDataDef.h"
#include "../NetSinkObj.h"
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
	g_pConnectServer->DisconnectToServer(m_nPeerSerType, m_nPeerSerNo, m_pNet->GetServiceIndex());
	m_timer_Link.StopTimer();
	m_timer_reconnect.StartTimerSec(CLIENT_RECONN_TIME);
	return true;
}

void CConnCliSink::Close()
{
	
}


bool CConnCliSink::HandNetData(uint16 nMain, uint16 nSub, CInputPacket& inPacket)
{
	m_pNet->Log("Recv cmd %d, %d",nMain, nSub);
	switch (nMain)
	{
		case MAIN_MSG_CENTERSER:
		{
			return HandMsgFromCenterSrv(nSub, inPacket);
		}
		case MAIN_MSG_USERSER:
		{
			return HandMsgFromUserSrv(nSub, inPacket);		
		}
		case MAIN_MSG_GAMESER:
		{
			return HandMsgFromGameSrv(nSub,  inPacket);
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
		COutputPacket out;
		out.Begin(MAIN_MSG_CONNSER, CS_SUB_MSG_TEST);
		out.End();
		CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), out);
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

void CConnCliSink::RegConnSrv()
{
	COutputPacket out;
	out.Begin(MAIN_MSG_CONNSER, CS_SUB_MSG_REG_CONN);
	out.WriteInt16(g_pConnectServer->GetSerNo());
	out.End();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(),out);
}

void CConnCliSink::ConnectSucess(CInputPacket& inPacket)
{
	m_nReConnectCount = 0;
	m_nPeerSerType = inPacket.ReadInt16();
	m_nPeerSerNo = inPacket.ReadInt16();
	m_timer_Link.StartTimerSec(CLIENT_TEST_TIME);
}

void CConnCliSink::UploadSrvInfo()
{
	COutputPacket out;
	out.Begin(MAIN_MSG_CONNSER, CS_SUB_MSG_UPLOADINFO);
	out.WriteInt32(Single_Get(CCore)->GetServiceNum());
	out.End();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(),out);
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
			UploadSrvInfo();
			return true;
		}
		case CT_SUB_MSG_NEWGAMESER:
		{
			uint16 nNum = inPacket.ReadInt16();
			for(int i = 0; i < nNum; i++)
			{
				GameInfo info;
				info.nGameNo = inPacket.ReadInt16();
				info.szIp = inPacket.ReadString();
				info.nPort = inPacket.ReadInt16();
				info.nGameId = inPacket.ReadInt32();
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
			SERVICEINDEX nIndex = inPacket.ReadInt16();
			uint32 nDataSize = inPacket.ReadInt32();
			char* pBinary = (char*)inPacket.ReadBinary(nDataSize);
			CNetSinkObj::SendData(m_pNet, nIndex, pBinary, nDataSize);
			return true;
		}
		case GS_SUB_MSG_GAME2CONN:
		{
			//InnserSync* pSync = (InnserSync*)pData;
			//m_pNet->PostData(pSync->nIndex, pSync->nType, pSync+1, nDataSize - sizeof(InnserSync));
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
			ConnectSucess(inPacket);
			break;
		}
		case US_SUB_MSG_TEST:
		{
			m_nTestNum = 0;
			break;
		}
		case US_SUB_MSG_MEM_BASE_RET:
		{
			SERVICEINDEX nIndex = inPacket.ReadInt16();
			m_pNet->PostData(nIndex, MEM_DATA_BASE_RET, inPacket.RestPacket(), inPacket.Rest_Len());
			break;
		}
		default:
			break;
	}
	return true;
}

