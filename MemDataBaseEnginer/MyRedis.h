#pragma once
#include <stdlib.h>
#include "../include/hiredis/hiredis.h"
#include "../include/types.h"
#include <string>

struct RedisConfig
{
	std::string szHost;
	unsigned short nPort;
	std::string szAuth;

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
	virtual int  Exec(UINT nType,void* pData,UINT nDataSize,void *pRet=NULL,UINT nRet=0)=0;
protected:
	virtual int  GetRedisConfig()=0;
	virtual bool Connected()=0;
	bool OpenConnect();
	void CloseConnect();
	void CloseReply();
protected:
	redisContext* m_pConn;
	redisReply* m_pReply;
	RedisConfig m_dbConfig;
};
