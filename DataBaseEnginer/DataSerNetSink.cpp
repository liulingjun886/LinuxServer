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
#include "DataBaseEnginer.h"
#include "MemDataBaseEnger.h"


CDataSerNetSink::CDataSerNetSink(CServices* pNet) :CNetHandSink(pNet),m_nTestLink(0)
{

}


CDataSerNetSink::~CDataSerNetSink()
{
	
}

void CDataSerNetSink::Connect()
{
	//m_pNet->SetTimer(TIME_CONN_IS_LINK, 100*60, -1);
	CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_DATA_SUCSS);
}

bool CDataSerNetSink::HandNetData(USHORT nIndex,USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	switch (nMain)
	{
	case MAIN_MSG_NET:
	{
		return HandMainMsgNet(nIndex,nSub,pData,nDataSize);
	}
	case MAIN_MSG_DATACENTER:
	{
		return HandMainMsgCenter(nIndex,nSub,pData,nDataSize);
	}
	default:
		break;
	}
	return false;
}

bool CDataSerNetSink::HandTimeMsg(USHORT uTimeId)
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

bool CDataSerNetSink::TestNetLink()
{
	if (m_nTestLink > 0)
	{
	  	return DisConnect();
	}
	++m_nTestLink;
	CNetSinkObj::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_TEST);
	return true;
}

bool CDataSerNetSink::HandMainMsgNet(USHORT nIndex,USHORT nSub, void* pData, USHORT nDataSize)
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


bool CDataSerNetSink::HandMainMsgCenter(USHORT nIndex,USHORT nSub,void* pData,USHORT nDataSize)
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

