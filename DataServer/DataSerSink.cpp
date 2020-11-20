#include <stdio.h>
#include "DataSerNetSink.h"
#include "Core.h"
#include "ToolLock.h"
#include "commproto.h"
#include "Services.h"
#include "MemDataDef.h"
#include "DataBaseDef.h"
#include "NetSinkObj.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../DataBaseEnginer/DataBaseEnginer.h"
#include "../MemDataBaseEnginer/MemDataBaseEnger.h"

extern CDataServer* g_pDataServer;

CDataSerSink::CDataSerSink(CServices* pNet) :CNetHandSink(pNet),m_nTestLink(0)
{

}


CDataSerSink::~CDataSerSink()
{
	
}

void CDataSerSink::Connect()
{
	//m_pNet->SetTimer(TIME_CONN_IS_LINK, 100*60, -1);
	CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_DATASER, DS_SUB_MSG_CONN_SUCSS);
}

bool CDataSerSink::HandNetData(USHORT nIndex,USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	switch (nMain)
	{
	case MAIN_MSG_NET:
	{
		return HandMainMsgNet(nIndex,nSub,pData,nDataSize);
	}
	case MAIN_MSG_GAMESER:
	{
		return HandMainMsgGameSer(nIndex,nSub,pData,nDataSize);
	}
	default:
		break;
	}
	return false;
}

bool CDataSerSink::HandTimeMsg(USHORT uTimeId)
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
	CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_TEST);
	return true;
}

bool CDataSerSink::HandMainMsgNet(USHORT nIndex,USHORT nSub, void* pData, USHORT nDataSize)
{
	switch (nSub)
	{
		 case SUB_MSG_TEST:
		 {
			m_nTestLink = 0;
			return true;
		 }
		 default:
			 return false;
	}
	return true;
}


bool CDataSerSink::HandMainMsgGameSer(USHORT nIndex,USHORT nSub,void* pData,USHORT nDataSize)
{
	switch (nSub)
	{
		case SUB_MSG_MEM_DATA_BASE_REQ:
		{
			CMemDataBaseEnginer::PostMemDataBaseReq(m_pNet, pData, nDataSize);
			break;
		}
		case SUB_MSG_DATA_BASE_REQ:
		{
			CDataBaseEnginer::PostDataBaseReq(m_pNet,pData, nDataSize);
			break;
		}
		default:
			return false;
	}
	return true;
}

