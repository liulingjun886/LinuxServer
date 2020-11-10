#pragma once
#include <stdlib.h>
#include "hiredis/hiredis.h"
#include "types.h"

#define MAX_REDIS_SCRIPT 5

enum Redis_Pro
{
	PRO_LOGIN_IN,
	PRO_LOGIN_OUT,
	PRO_JOIN_GAME,
	PRO_QUIT_GAME,
	PRO_MAX,
};

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

class CMyRedis
{
public:
	CMyRedis();
	~CMyRedis();
public:
	bool Init();
	bool Exec(UINT nType,void* pData,UINT nDataSize,void *pRet=NULL,UINT nRet=0);
	void CloseReply();
private:
	bool OpenConnect();
	void CloseConnect();
	
	bool CreateScript();
	void RegRedisScript(int nType,const char* szStr);
	void UserLoginReq(int nUserId,USHORT nSerNo,SERVICEINDEX nSevNo,void* pRet=NULL,UINT nRet=0);
	void UserJoinGame(int nUserId,USHORT nSerNo,SERVICEINDEX nSevNo,int nSeatNo,void* pRet=NULL,UINT nRet=0);
private:
	char* m_RedisPro[MAX_REDIS_SCRIPT];
	redisContext* m_pConn;
	redisReply* m_pReply;
	RedisConfig m_dbConfig;
};
