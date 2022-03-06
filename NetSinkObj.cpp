#include "NetSinkObj.h"
#include "./include/core/Services.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Server.h"
#include "NetHandSink.h"


extern CServer* g_pSer;

CNetSinkObj::CNetSinkObj(CNetHandSink* pSink):
	m_pSink(pSink)
{
	
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


