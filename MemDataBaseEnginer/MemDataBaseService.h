#pragma once
#include "../include/Services.h"
#include "../include/types.h"
//#include "interface.h"

class CMemSink;

class CMemDataBaseService : public CServices
{
	friend class CMemDataBaseEnginer;
	CMemSink* m_Sink;
public:
	CMemDataBaseService();
	~CMemDataBaseService();
	bool Init();
protected:
	virtual bool HandData(int e, SERVICEINDEX uFromSerId, void *pData, DATASIZE size);
};

