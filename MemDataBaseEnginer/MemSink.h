#pragma once
#include "../include/core/Services.h"

class CRedis;

class CMemSink
{
public:
	CMemSink(CServices* pMemData);
	~CMemSink();
public:
	bool Init();
	bool HandMemDataReq(SERVICEINDEX uFromSerId, void *pData, DATASIZE size);
	bool HandTimeMsg(TIMEERID nTimeId);
private:
	CRedis* m_pMemData;
};

