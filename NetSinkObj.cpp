#include "NetSinkObj.h"
#include "./include/core/Services.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "./CenterServer/CenSerSink.h"
#include "./UserServer/UserCliSink.h"
#include "./UserServer/UserSerSink.h"
#include "./DataServer/DataSerSink.h"
#include "./DataServer/DataCliSink.h"
#include "./GameServer/GameSerSink.h"
#include "./GameServer/GameCliSink.h"
#include "./ConntionServer/ConnSerSink.h"
#include "./ConntionServer/ConnCliSink.h"

extern CServer* g_pSer;

CNetSinkObj::CNetSinkObj(CServices* pServices,int nConnType):
	m_pSink(NULL)
{
	switch (g_pSer->GetSerType())
	{
		case 1: //中心服
		{
			m_pSink = new CCenSerSink(pServices);
			break;
		}
		case 2: //玩家服务器
		{
			if(SOCK_CONN_SER == nConnType)
				m_pSink = new CUserSerSink(pServices);
			else
				m_pSink = new CUserCliSink(pServices);
			break;
		}
		case 3: //数据服务器
		{
			if(SOCK_CONN_SER == nConnType)
				m_pSink = new CDataSerSink(pServices);
			else
				m_pSink = new CDataCliSink(pServices);
			break;
		}
		case 4: //游戏服务器
		{
			if(SOCK_CONN_SER == nConnType)
				m_pSink = new CGameSerSink(pServices);
			else
				m_pSink = new CGameCliSink(pServices);
			break;
		}
		case 5: //连接服务器
		{
			if(SOCK_CONN_SER == nConnType)
				m_pSink = new CConnSerSink(pServices);
			else
				m_pSink = new CConnCliSink(pServices);
			break;
		}
	}
	
	assert(m_pSink != NULL);
}

CNetSinkObj::~CNetSinkObj()
{
	SAFE_DELTE(m_pSink)
}

void CNetSinkObj::SendData(CServices* pService,SERVICEINDEX nIndex, void* pData,DATASIZE nDataSize)
{
	if(nIndex == INVALID_SERIVCE_INDEX)
		return;
	pService->PostData(nIndex, SEND_DATA_REQ, pData, nDataSize);
	
}

void CNetSinkObj::SendData(CServices* pService,SERVICEINDEX nIndex, COutputPacket& out)
{
	if(nIndex == INVALID_SERIVCE_INDEX)
		return;

	if(out.Packet_Len() > MAX_MSG_SIZE)
		return;

	pService->PostData(nIndex, SEND_DATA_REQ, out.Get_Packet(), out.Packet_Len());
}


/*
	-1 出错
	0	没有处理继续接收
	int 处理了多少字节
*/
int CNetSinkObj::HandNetMsg(SERVICEINDEX nIndex, const char* pData, unsigned nDataCount)
{
	int32 nPacketLen = CInputPacket::GetPacketLen(pData, nDataCount);
	
	if(nPacketLen > 0)
	{
		CInputPacket in;
		in.Copy(pData, nPacketLen);
		if(!m_pSink->HandNetData(in.GetMainCmd(), in.GetSubCmd(), in))
		{
			return -1;
		}
	}
	return nPacketLen;
}


bool CNetSinkObj::HandTimeMsg(TIMEERID uTimeID)
{
	return m_pSink->HandTimeMsg(uTimeID);
}

bool CNetSinkObj::DisConnect()
{
	return m_pSink->DisConnect();
}

void CNetSinkObj::Init(const char* szIp)
{
	return m_pSink->Init(szIp);
}

void CNetSinkObj::Close()
{
	m_pSink->Close();
}

void CNetSinkObj::Connect()
{
	m_pSink->Connect();
}

bool CNetSinkObj::HandUserMsg(int nType, void* pData, DATASIZE uDataSize)
{
	return m_pSink->HandUserMsg(nType, pData, uDataSize);
}


