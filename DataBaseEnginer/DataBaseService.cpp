#include "DataBaseService.h"
#include "DataBaseSink.h"
#include <stdio.h>
CDataBaseService::CDataBaseService() : m_pSink(NULL)
{
	m_pSink = new CDataBaseSink(this);
}
CDataBaseService::~CDataBaseService()
{
	SAFE_DELTE(m_pSink)
}

bool CDataBaseService::Init()
{
	return m_pSink->Init();
}


bool CDataBaseService::HandData(int e, SERVICEINDEX uFromSerId, void *pData, DATASIZE size)
{
	switch (e)
	{
		case TIME_MSG:
		{
			uint32 nTimeId = *(uint32*)pData;
			return m_pSink->HandTimeMsg(nTimeId);
		}
		case DATA_BASE_REQ:
		{
			DataCenter* pCenter = (DataCenter*)pData;
			uint32* pType = (uint32*)(pCenter+1);
			return m_pSink->HandDataBaseReq(uFromSerId,pCenter->nCsid,*pType,pType+1,size-sizeof(uint32)-sizeof(DataCenter));
		}
		default:
			break;
	}
	return true;
}
