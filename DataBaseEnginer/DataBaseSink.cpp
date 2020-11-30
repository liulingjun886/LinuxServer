#include "DataBaseSink.h"
#include <stdio.h>
#include "../commproto.h"
#include "../include/Services.h"
#include "DataBaseEnginer.h"
#include "../DataServer/DataMySql.h"


CDataBaseSink::CDataBaseSink(CServices    * pService)
{
	m_pDataBase = new CDataMySql(pService);
}

bool CDataBaseSink::Init()
{
	if(!m_pDataBase->InitConnection())
	{
		printf("m_pDataBase->Init() false\n");
		return false;
	}
	return true;
}

CDataBaseSink::~CDataBaseSink()
{
	SAFE_DELTE(m_pDataBase);
}

bool CDataBaseSink::HandDataBaseReq(SERVICEINDEX nFromIndex,SERVICEINDEX nCsid, uint32 nType,void* pData,DATASIZE nDataSize)
{
	return m_pDataBase->Exec(nFromIndex, nCsid, nType, pData, nDataSize);
}

bool CDataBaseSink::HandTimeMsg(TIMEERID nTimeId)
{
	return true;
}
