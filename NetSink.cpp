#include "./include/core/NetSink.h"
#include "./include/core/Services.h"
#include "NetSinkObj.h"

CNetSink::CNetSink()
{
	
}

CNetSink::~CNetSink()
{
	
}

CNetSink* CNetSink::CreateNetSink(CServices* pServices,int nType)
{
	return new CNetSinkObj(pServices,nType);
}

void CNetSink::DestroyNetSink(CNetSink* pSink)
{
	delete pSink;
}

