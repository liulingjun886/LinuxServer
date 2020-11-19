#include "UserSerSink.h"

extern CServer* g_pSer;

CUserSerSink::CUserSerSink(CServices* pServices):CNetHandSink(pServices)
{
	m_pServer = dynamic_cast<CUserServer*>(g_pSer);
}

CUserSerSink::~CUserSerSink()
{
	
}

bool CUserSerSink::HandTimeMsg(USHORT uTimeID)
{
	
}

bool CUserSerSink::HandNetData(USHORT nSrcIndex, USHORT nMain, USHORT nSub, void* pData, UINT nDataSize)
{
	switch(nMain)
	{
		case MAIN_MSG_NET:
			{
				m_nNum = 0;
				CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_TEST);
				return true;
			}
		case MAIN_MSG_GAMESER:
			return HandMainMSgGameSer(nSrcIndex,nSub,pData,nDataSize);
		case MAIN_MSG_CONNECT:
			return HandMainMsgConnSer(nSrcIndex,nSub,pData,nDataSize);
		default:
			break;
	}
	return false;
}

void CUserSerSink::Connect()
{
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_USERSER, US_SUB_MSG_CONN_SUCSS);
}

bool CUserSerSink::HandMainMSgGameSer(USHORT nSrcIndex, USHORT nSub, void* pData, UINT nDataSize)
{
	
	return true;
}

bool CUserSerSink::HandMainMsgConnSer(USHORT nSrcIndex, USHORT nSub, void* pData, UINT nDataSize)
{
	return true;
}

