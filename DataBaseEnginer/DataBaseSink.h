#pragma once
#include "types.h"

class IDataBase;
class CServices;

class CDataBaseSink
{
public:
	CDataBaseSink(CServices*     pDataBaseService);
	~CDataBaseSink();
public:
	bool Init();
	bool HandDataBaseReq(SERVICEINDEX nFromIndex,SERVICEINDEX nCsid,UINT nType,void* pData,DATASIZE nDataSize);
	bool HandTimeMsg(TIMEERID nTimeId);
private:
	void UserLogin(USHORT nIndex,SERVICEINDEX nCsid,void* pData,USHORT nDataSize);
private:
	CServices* const m_pService;
	IDataBase* m_pDataBase;
};