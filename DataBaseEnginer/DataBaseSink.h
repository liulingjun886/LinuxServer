#pragma once
#include "../Defines.h"

class CMySql;
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
	CMySql* m_pDataBase;
};