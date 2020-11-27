#include "GameSerSink.h"
#include "../commproto.h"
#include "GameServer.h"
#include "../include/Services.h"
#include "../NetSinkObj.h"

extern CGameServer* g_pGameServer;

CGameSerSink::CGameSerSink(CServices* pServices):CNetHandSink(pServices)
{
	
}

CGameSerSink::~CGameSerSink()
{
	
}

void CGameSerSink::Connect()
{
	ConnSucess conn;
	conn.nSrvNo = g_pGameServer->GetSerNo();
	conn.nSrvType = g_pGameServer->GetSerType();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_GAMESER, GS_SUB_MSG_CONN_SUCSS, &conn, sizeof(conn));
}

void CGameSerSink::Close()
{
	g_pGameServer->DelConnSrvIndex(m_nConnNo, m_pNet->GetServiceIndex());
}

bool CGameSerSink::HandTimeMsg(uint16 uTimeID)
{
	return true;
}


bool CGameSerSink::HandNetData(uint16 nIndex, uint16 nMain, uint16 nSub, void* pData, uint32 nDataSize)
{
	switch(nMain)
	{
		case MAIN_MSG_CONNSER:
			return HandMsgFromConnSrv(nIndex, nSub, pData, nDataSize);
		default:
			break;
	}
	return false;
}

bool CGameSerSink::HandTestNetConn()
{
	m_nTestNum = 0;
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_GAMESER, GS_SUB_MSG_TEST);
	return true;
}


bool CGameSerSink::HandMsgFromConnSrv(uint16 nIndex, uint16 nSub, void* pData, uint32 nDataSize)
{
	switch(nSub)
	{
		case CS_SUB_MSG_REG_CONN:
		{
			RegConnSer* pReg = (RegConnSer*)pData;
			m_nConnNo = pReg->nSerNo;
			g_pGameServer->AddConnInfo(pReg->nSerNo, m_pNet->GetServiceIndex());
			return true;
		}
		case CS_SUB_MSG_TEST:
		{
			
			return true;
		}
		default:
			break;
	}
	return true;
}

