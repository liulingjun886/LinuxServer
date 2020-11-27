#pragma once
#include <stdlib.h>
#include "../include/hiredis/hiredis.h"
#include "../Defines.h"
#include <string>

struct RedisConfig
{
	std::string szHost;
	unsigned short nPort;
	std::string szAuth;

	RedisConfig()
	{
		nPort = 0;
		szHost = "";
		szAuth = "";
	}
};

class CRedis
{
public:
	CRedis();
	virtual ~CRedis();
public:
	bool InitConnection();
	virtual int  Exec(uint32 nType,void* pData,uint32 nDataSize,void *pRet=NULL,uint32 nRet=0)=0;
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
