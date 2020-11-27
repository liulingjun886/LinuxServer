#pragma once

#include "../MemDataBaseEnginer/MyRedis.h"
#include "../Defines.h"

class CDataRedis : public CRedis
{
public:
	CDataRedis();
	~CDataRedis();
protected:
	virtual int  GetRedisConfig();
	virtual bool Connected();
	virtual int  Exec(uint32 nType,void* pData,uint32 nDataSize,void *pRet=NULL,uint32 nRet=0);
private:
	void RegRedisScript(int nType,const char* szStr);
private:
	char* m_RedisPro[5];
};