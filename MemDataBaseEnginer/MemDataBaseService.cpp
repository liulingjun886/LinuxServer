#include "MemDataBaseService.h"
#include "MemDataBaseEnger.h"
#include <assert.h>
#include "MemSink.h"

CMemDataBaseService::CMemDataBaseService() :m_Sink(NULL)
{
	m_Sink = new CMemSink(this);
	assert(m_Sink != NULL);
}

CMemDataBaseService::~CMemDataBaseService()
{
	SAFE_DELTE(m_Sink);
}

bool CMemDataBaseService::Init()
{
	return m_Sink->Init();
}

bool CMemDataBaseService::HandData(int e, SERVICEINDEX uFromSerId, void *pData, DATASIZE uDataSize)
{
	switch (e)
	{
	case TIME_MSG:
	{
		TIMEERID nTimeId = *(TIMEERID*)pData;
		return m_Sink->HandTimeMsg(nTimeId);
	}
	case MEM_DATA_BASE_REQ:
	{
		static DATASIZE nHeadSize = sizeof(DataCenter) + sizeof(uint32);
		if(uDataSize < nHeadSize)
			break;
		
		DataCenter* pCsid = (DataCenter*)pData;
		uint32* pType = (uint32*)(pCsid+1);
		m_Sink->HandMemDataReq(uFromSerId,pCsid->nCsid,*pType ,pType+1, uDataSize-nHeadSize);
		break;
	}
	default:
		break;
	}
	return true;
}

