#include "UserSerSink.h"
#include "../NetSinkObj.h"
#include "UserServer.h"
#include "../include/Services.h"
#include "../commproto.h"

extern CUserServer* g_pUserServer;

CUserSerSink::CUserSerSink(CServices* pServices):CNetHandSink(pServices)
{
	
}

CUserSerSink::~CUserSerSink()
{
	
}

bool CUserSerSink::HandTimeMsg(uint16 uTimeID)
{
	return true;
}

bool CUserSerSink::HandNetData(uint16 nSrcIndex, uint16 nMain, uint16 nSub, void* pData, uint32 nDataSize)
{
	switch(nMain)
	{
		case MAIN_MSG_GAMESER:
			return HandMainMSgGameSer(nSrcIndex,nSub,pData,nDataSize);
		case MAIN_MSG_CONNSER:
			return HandMainMsgConnSer(nSrcIndex,nSub,pData,nDataSize);
		default:
			break;
	}
	return false;
}

bool CUserSerSink::HandTestNetConn()
{
	m_nNum = 0;
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_USERSER, US_SUB_MSG_TEST);
	return true;
}


void CUserSerSink::Connect()
{
	ConnSucess conn;
	conn.nSrvNo = g_pUserServer->GetSerNo();
	conn.nSrvType = g_pUserServer->GetSerType();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_USERSER, US_SUB_MSG_CONN_SUCSS,&conn,sizeof(conn));
}

bool CUserSerSink::HandMainMSgGameSer(uint16 nSrcIndex, uint16 nSub, void* pData, uint32 nDataSize)
{
	switch(nSub)
	{
		case GS_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
	}
	return true;
}

bool CUserSerSink::HandMainMsgConnSer(uint16 nSrcIndex, uint16 nSub, void* pData, uint32 nDataSize)
{
	switch(nSub)
	{
		case CS_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
	}
	return true;
}

