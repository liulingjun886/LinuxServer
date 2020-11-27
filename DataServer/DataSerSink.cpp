#include <stdio.h>
#include "DataSerSink.h"
#include "../include/Core.h"
#include "../include/ToolLock.h"
#include "../commproto.h"
#include "../include/Services.h"
#include "../MemDataBaseEnginer/MemDataDef.h"
#include "../DataBaseEnginer/DataBaseDef.h"
#include "../NetSinkObj.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../DataBaseEnginer/DataBaseEnginer.h"
#include "../MemDataBaseEnginer/MemDataBaseEnger.h"
#include "DataServer.h"

extern CDataServer* g_pDataServer;

enum TIME_ID
{
	TIME_CONN_IS_LINK = 1,
};

CDataSerSink::CDataSerSink(CServices* pNet) :CNetHandSink(pNet),m_nTestLink(0)
{

}


CDataSerSink::~CDataSerSink()
{
	
}

void CDataSerSink::Connect()
{
	//m_pNet->SetTimer(TIME_CONN_IS_LINK, 100*60, -1);
	ConnSucess conn;
	conn.nSrvNo = g_pDataServer->GetSerNo();
	conn.nSrvType = g_pDataServer->GetSerType();
	CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_DATASER, DS_SUB_MSG_CONN_SUCSS,&conn,sizeof(conn));
}

bool CDataSerSink::HandNetData(uint16 nIndex,uint16 nMain, uint16 nSub, void* pData, uint16 nDataSize)
{
	switch (nMain)
	{
	case MAIN_MSG_GAMESER:
	{
		return HandMainMsgFromGameSer(nIndex,nSub,pData,nDataSize);
	}
	default:
		break;
	}
	return false;
}

bool CDataSerSink::HandTimeMsg(uint16 uTimeId)
{
	switch (uTimeId)
	{
	case TIME_CONN_IS_LINK:
	{
		return TestNetLink();
	}
	default:
		break;
	}
	return true;
}

bool CDataSerSink::TestNetLink()
{
	if (m_nTestLink > 0)
	{
	  	return DisConnect();
	}
	++m_nTestLink;
	
	return true;
}

bool CDataSerSink::HandMainMsgFromGameSer(uint16 nIndex,uint16 nSub,void* pData,uint16 nDataSize)
{
	switch (nSub)
	{
		case GS_SUB_MSG_TEST:
		{
			m_nTestLink = 0;
			CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_DATASER, DS_SUB_MSG_TEST);
			return true;
		}
		//case SUB_MSG_MEM_DATA_BASE_REQ:
		//{
		//	CMemDataBaseEnginer::PostMemDataBaseReq(m_pNet, pData, nDataSize);
		//	break;
		//}
		//case SUB_MSG_DATA_BASE_REQ:
		//{
		//	CDataBaseEnginer::PostDataBaseReq(m_pNet,pData, nDataSize);
		//	break;
		//}
		default:
			return false;
	}
	return true;
}

