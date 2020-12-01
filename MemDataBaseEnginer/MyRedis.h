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

class CServices;

class CRedis
{
public:
	CRedis(CServices* pService);
	virtual ~CRedis();
public:
	bool InitConnection();
	virtual int  Exec(SERVICEINDEX nSrcIndex,SERVICEINDEX nCsid,uint32 nType,void* pData,DATASIZE nDataSize)=0;
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
	CServices* const m_pService;
};
