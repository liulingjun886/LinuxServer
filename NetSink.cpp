#include "NetSink.h"
#include "Services.h"
#include "ConnSerNetSink.h"
#include "ConnCliNetSink.h"
#include "DataSerNetSink.h"
#include "GameCliNetSink.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

extern USHORT g_nSerType;


USHORT CNetSink::nHeadSize = sizeof(NetHead);
USHORT CNetSink::nCommandSize = sizeof(NetMsgCommand);
USHORT CNetSink::nMinDataSize = CNetSink::nHeadSize + CNetSink::nCommandSize;

CNetSink::CNetSink(CServices* pServices,int nType):m_pSink(NULL)
{
	switch (g_nSerType)
	{
		case 1:
		{
			m_pSink = new CDataSerNetSink(pServices);
			break;
		}
		case 2:
		{
			if(nType == 1)
				m_pSink = new CConnSerNetSink(pServices);
			else
				m_pSink = new CConnCliNetSink(pServices);
			break;
		}
		case 3:
		{
			m_pSink = new CGameCliNetSink(pServices);
			break;
		}
	}
	assert(m_pSink != NULL);
}

CNetSink::~CNetSink()
{
	SAFE_DELTE(m_pSink)
}

void CNetSink::SendData(CServices* pService,SERVICEINDEX nIndex, USHORT nMain,USHORT nSub,void* pData,DATASIZE nDataSize)
{
	if(nIndex == INVALID_SERIVCE_INDEX)
		return;
	
	DATASIZE nPacketSize = nMinDataSize+ nDataSize;
	if (nPacketSize > MAX_MSG_SIZE)
		return;

	char buff[MAX_MSG_SIZE] = {0};
	NetHead* pHead = (NetHead*)buff;
	pHead->uDataSize = nPacketSize;
	NetMsgCommand* pCom = (NetMsgCommand*)(pHead+1);
	pCom->uMain = nMain;
	pCom->uSub = nSub;
	if (nDataSize > 0)
	{
		memcpy(pCom + 1, pData, nDataSize);
	}
	pService->PostData(nIndex, SEND_DATA_REQ, buff,nPacketSize);
}

/*
	-1 出错
	0	没有处理继续接收
	int 处理了多少字节
*/
int CNetSink::HandNetMsg(SERVICEINDEX nIndex, void* pData, DATASIZE nDataSize)
{
	if(nDataSize <= nHeadSize)
		return 0;
	
	NetHead* pNetHead = (NetHead*)pData;
	DATASIZE uPacketSize = pNetHead->uDataSize;
	
	if (uPacketSize > MAX_MSG_SIZE)
	{
		printf("uPacketSize = %d\n",uPacketSize);
		return -1;
	}
	if (nDataSize < uPacketSize)
		return 0;
	
	NetMsgCommand * pCom = (NetMsgCommand *)(pNetHead+1);
	DATASIZE nDataCount = uPacketSize - nMinDataSize;
	if(!m_pSink->HandNetData(nIndex, pCom->uMain, pCom->uSub, pCom+1, nDataCount))
	{
		printf("nMain = %d,nSub = %d\n",pCom->uMain, pCom->uSub);
		return -1;
	}
	return uPacketSize;
}

bool CNetSink::HandTimeMsg(TIMEERID uTimeID)
{
	return m_pSink->HandTimeMsg(uTimeID);
}
bool CNetSink::DisConnect()
{
	return m_pSink->DisConnect();
}

void CNetSink::Init(UINT nIp)
{
	return m_pSink->Init(nIp);
}

void CNetSink::Init(const char* szIp)
{
	return m_pSink->Init(szIp);
}

void CNetSink::Close()
{
	m_pSink->Close();
}

void CNetSink::Connect()
{
	m_pSink->Connect();
}

bool CNetSink::HandUserMsg(int nType, void* pData, DATASIZE uDataSize)
{
	return m_pSink->HandUserMsg(nType, pData, uDataSize);
}
