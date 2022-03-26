#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "MemDataBaseEnger.h"
#include "MemSink.h"
#include "MemDataBaseService.h"
#include <unistd.h>
#include "../include/core/Core.h"
#include "../include/core/SingleObject.h"
#include "../NetSinkObj.h"

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
		if (!CSingleObject<CCore>::Instance()->AddService(pService))
		{
			printf("MemDataBaseService Create Failer\n");
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

SERVICEINDEX CMemDataBaseEnginer::GetIndex(SERVICEINDEX nRand)
{
	if(m_nServiceNum == 0)
		return 0;

	return m_pIndex[nRand%m_nServiceNum];
}
