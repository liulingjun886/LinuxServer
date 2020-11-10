#pragma once
#include "NetHandSink.h"
#include "DataSerManager.h"

class CServices;
class CCliNetSink : public CNetHandSink
{
	static CDataSerManager m_sDataSer;
public:
	CCliNetSink(CServices*);
	~CCliNetSink();
public:
	static SERVICEINDEX GetDataSer(int nRand);
	static void PostDataBaseReq(CServices* pServices,UINT nType,void* pData, DATASIZE uDataSize);
	static void PostMemDataBaseReq(CServices* pServices,UINT nType,void* pData, DATASIZE uDataSize);
protected:
	void AddDataSer();
	void DelDataSer();
};