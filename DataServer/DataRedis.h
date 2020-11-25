#pragma once

#include "../MemDataBaseEnginer/MyRedis.h"
#include "../include/types.h"

class CDataRedis : public CRedis
{
public:
	CDataRedis();
	~CDataRedis();
protected:
	virtual int  GetRedisConfig();
	virtual bool Connected();
	virtual int  Exec(UINT nType,void* pData,UINT nDataSize,void *pRet=NULL,UINT nRet=0);
private:
	void RegRedisScript(int nType,const char* szStr);
private:
	char* m_RedisPro[5];
};