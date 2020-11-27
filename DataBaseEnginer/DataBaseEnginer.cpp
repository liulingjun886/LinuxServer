#include <stdio.h>
#include "DataBaseEnginer.h"
#include "../include/Services.h"
#include "DataBaseService.h"
#include "../include/Core.h"
#include <unistd.h>
#include <stdlib.h>
#include "../NetSinkObj.h"

Single_Init(CDataBaseEnginer)
CDataBaseEnginer::CDataBaseEnginer() : m_nServiceNum(0), 
m_pIndex(NULL)
{
	
}

CDataBaseEnginer::~CDataBaseEnginer()
{
	SAFE_DELTEARRAY(m_pIndex);
}

void CDataBaseEnginer::SetServiceNum(int nNum)
{
	if(nNum <= 0)
		return;
	m_nServiceNum = nNum;
	m_pIndex = new SERVICEINDEX[m_nServiceNum];

	for(int i = 0; i < m_nServiceNum; i++)
	{
		CDataBaseService* pService = new CDataBaseService;
		if (!pService || !Single_Get(CCore)->AddService(pService))
		{
			exit(0);
		}

		if(!pService->Init())
		{
			printf("DataBaseService Init Failer\n");
			exit(0);
		}
		m_pIndex[i] = pService->GetServiceIndex();
	}
}

inline SERVICEINDEX CDataBaseEnginer::GetIndex(int nRand)
{
	if(m_nServiceNum == 0)
		return 0;

	return m_pIndex[nRand%m_nServiceNum];
}

void CDataBaseEnginer::PostDataBaseReq(CServices* pServices,void* pData, DATASIZE uDataSize)
{
	SERVICEINDEX nIndex = Single_Get(CDataBaseEnginer)->GetIndex(pServices->GetServiceIndex());
	if(nIndex == INVALID_SERIVCE_INDEX)
		return;
	
	pServices->PostData(nIndex, DATA_BASE_REQ, pData, uDataSize);
}

void CDataBaseEnginer::PostDataBaseRet(CServices* pServices,SERVICEINDEX nToSerId,SERVICEINDEX nCsid, uint32 uTypeId, void* pData, DATASIZE uDataSize)
{
	//static DATASIZE nHeadSize = sizeof(DataCenter) + sizeof(uint32);
	char* buff[MAX_MSG_SIZE] = {0};
	DataCenter* pCenter = (DataCenter*)buff;
	pCenter->nCsid = nCsid;
	uint32* pType = (uint32*)(pCenter+1);
	*pType = uTypeId;
	if (uDataSize > 0)
	{
		memcpy(pType+1, pData, uDataSize);
	}
	CNetSinkObj::SendData(pServices,  nToSerId, MAIN_MSG_USERSER,SUB_MSG_DATA_BASE_RET , buff, uDataSize+sizeof(uint32));
}


