#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "MemDataBaseEnger.h"
#include "MemSink.h"
#include "MemDataBaseService.h"
#include <unistd.h>
#include "Core.h"
#include "NetSink.h"

Single_Init(CMemDataBaseEnginer)

CMemDataBaseEnginer::CMemDataBaseEnginer() : m_nServiceNum(0), m_pIndex(NULL)
{
	
}

CMemDataBaseEnginer::~CMemDataBaseEnginer()
{
	SAFE_DELTEARRAY(m_pIndex)
}

void CMemDataBaseEnginer::SetServiceNum(int nNum)
{
	if(nNum <= 0)
		return;
	
	m_nServiceNum = nNum;
	m_pIndex = new SERVICEINDEX[m_nServiceNum];

	for (int i = 0; i < m_nServiceNum; i++)
	{
		CMemDataBaseService* pService = new CMemDataBaseService;
		assert(pService != NULL);
		if (!Single_Get(CCore)->AddService(pService))
		{
			exit(0);
		}
		
		if(!pService->Init())
		{
			printf("MemDataBaseService Init Failer\n");
			exit(0);
		}

		m_pIndex[i] = pService->GetServiceIndex();
	}
}

inline SERVICEINDEX CMemDataBaseEnginer::GetIndex(SERVICEINDEX nRand)
{
	if(m_nServiceNum == 0)
		return 0;

	return m_pIndex[nRand%m_nServiceNum];
}


void CMemDataBaseEnginer::PostMemDataBaseReq(CServices* pServices,void* pData, DATASIZE uDataSize)
{
	SERVICEINDEX nIndex = Single_Get(CMemDataBaseEnginer)->GetIndex(pServices->GetServiceIndex());
	if(nIndex == INVALID_SERIVCE_INDEX)
		return;
	pServices->PostData(nIndex, MEM_DATA_BASE_REQ, pData, uDataSize);
}

void CMemDataBaseEnginer::PostMemDataBaseRet(CServices* pServices,SERVICEINDEX nIndex,SERVICEINDEX nCsid, UINT uTypeId, void* pData, DATASIZE uDataSize)
{
	static DATASIZE nHeadSize = sizeof(DataCenter) + sizeof(UINT);
	char* buff[MAX_MSG_SIZE] = {0};
	DataCenter* pCenter = (DataCenter*)buff;
	pCenter->nCsid = nCsid;
	UINT* pType = (UINT*)(pCenter+1);
	*pType = uTypeId;
	if (uDataSize > 0)
	{
		memcpy(pType+1, pData, uDataSize);
	}
	CNetSink::SendData(pServices,  nIndex,MAIN_MSG_DATACENTER, SUB_MSG_MEM_DATA_BASE_RET,buff, nHeadSize + uDataSize);
}


