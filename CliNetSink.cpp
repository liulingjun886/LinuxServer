#include "CliNetSink.h"
#include "NetSinkObj.h"
#include "Services.h"

CDataSerManager CCliNetSink::m_sDataSer;
CCliNetSink::CCliNetSink(CServices* pServices):CNetHandSink(pServices)
{
	
}
CCliNetSink::~CCliNetSink()
{
	
}

void CCliNetSink::AddDataSer()
{
	m_sDataSer.AddDataSer(m_pNet->GetServiceIndex());
}
void CCliNetSink::DelDataSer()
{
	m_sDataSer.DelDataSer(m_pNet->GetServiceIndex());
}

SERVICEINDEX CCliNetSink::GetDataSer(int nRand)
{
	return m_sDataSer.GetDataSer(nRand);
}

void CCliNetSink::PostDataBaseReq(CServices* pServices,UINT nType,void* pData, DATASIZE uDataSize)
{
	char buff[MAX_MSG_SIZE] = {0};
	DataCenter* pCen = (DataCenter*)buff;
	pCen->nCsid = pServices->GetServiceIndex();
	UINT* pType = (UINT*)(pCen + 1);
	*pType = nType;
	if(uDataSize>0)
	{
		memcpy(pType+1,pData,uDataSize);
	}
	CNetSinkObj::SendData(pServices,  GetDataSer(pServices->GetServiceIndex()), MAIN_MSG_DATACENTER, SUB_MSG_DATA_BASE_REQ, buff, uDataSize+sizeof(DataCenter)+sizeof(UINT));
}

void CCliNetSink::PostMemDataBaseReq(CServices* pServices,UINT nType,void* pData, DATASIZE uDataSize)
{
	char buff[MAX_MSG_SIZE] = {0};
	DataCenter* pCen = (DataCenter*)buff;
	pCen->nCsid = pServices->GetServiceIndex();
	UINT* pType = (UINT*)(pCen + 1);
	*pType = nType;
	if(uDataSize>0)
	{
		memcpy(pType+1,pData,uDataSize);
	}
	CNetSinkObj::SendData(pServices,  GetDataSer(pServices->GetServiceIndex()), MAIN_MSG_DATACENTER, SUB_MSG_MEM_DATA_BASE_REQ, buff, uDataSize+sizeof(DataCenter)+sizeof(UINT));
}


