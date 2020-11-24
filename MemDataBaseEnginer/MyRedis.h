#pragma once
#include <stdlib.h>
#include "hiredis/hiredis.h"
#include "types.h"

struct RedisConfig
{
	char szHost[16];
	unsigned short nPort;
	char szAuth[16];

	RedisConfig()
	{
		memset(this,0,sizeof(RedisConfig));
	}
};

class CRedis
{
public:
	CRedis();
	virtual ~CRedis();
public:
	bool InitConnection();
	
protected:
	virtual int  GetRedisConfig()=0;
	virtual bool Connected()=0;
	virtual int  Exec(UINT nType,void* pData,UINT nDataSize,void *pRet=NULL,UINT nRet=0)=0;

	bool OpenConnect();
	void CloseConnect();
	void CloseReply();
protected:
	redisContext* m_pConn;
	redisReply* m_pReply;
	RedisConfig m_dbConfig;
};
