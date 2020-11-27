#pragma once
#include "../Defines.h"

class IDataBase;
class CServices;

class CDataBaseSink
{
public:
	CDataBaseSink(CServices*     pDataBaseService);
	~CDataBaseSink();
public:
	bool Init();
	bool HandDataBaseReq(SERVICEINDEX nFromIndex,SERVICEINDEX nCsid,uint32 nType,void* pData,DATASIZE nDataSize);
	bool HandTimeMsg(TIMEERID nTimeId);
private:
	void UserLogin(uint16 nIndex,SERVICEINDEX nCsid,void* pData,uint16 nDataSize);
private:
	CServices* const m_pService;
	IDataBase* m_pDataBase;
};