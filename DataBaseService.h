#pragma once
#include "Services.h"

class CDataBaseSink;

class CDataBaseService :
	public CServices
{
	friend class CDataBaseEnginer;
	CDataBaseSink* m_pSink;
	public:
		bool Init();
		bool HandData(int e, SERVICEINDEX uFromSerId, void *pData, DATASIZE size);
	private:
		CDataBaseService();
		~CDataBaseService();
};