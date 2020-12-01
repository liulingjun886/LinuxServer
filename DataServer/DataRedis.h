#pragma once

#include "../MemDataBaseEnginer/MyRedis.h"
#include "../Defines.h"

class CDataRedis : public CRedis
{
public:
	CDataRedis(CServices* pService);
	~CDataRedis();
protected:
	virtual int  GetRedisConfig();
	virtual bool Connected();
	virtual int  Exec(SERVICEINDEX nSrcIndex,SERVICEINDEX nCsid,uint32 nType,void* pData,DATASIZE nDataSize);
private:
	void RegRedisScript(int nType,const char* szStr);
private:
	char* m_RedisPro[5];
};