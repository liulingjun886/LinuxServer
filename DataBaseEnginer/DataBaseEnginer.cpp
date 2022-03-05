#include <stdio.h>
#include "DataBaseEnginer.h"
#include "../include/core/Services.h"
#include "DataBaseService.h"
#include "../include/core/Core.h"
#include <unistd.h>
#include <stdlib.h>
#include "../NetSinkObj.h"

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
		if (!pService)
		{
			exit(0);
		}

		if(!Single_Get(CCore)->AddService(pService))
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

SERVICEINDEX CDataBaseEnginer::GetIndex(int nRand)
{
	if(m_nServiceNum == 0)
		return 0;

	return m_pIndex[nRand%m_nServiceNum];
}

