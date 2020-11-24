#pragma once

#include "../MemDataBaseEnginer/MyRedis.h"

extern CUserServer g_pUserServer;

enum Redis_Pro
{
	PRO_LOGIN_IN,
	PRO_LOGIN_OUT,
	PRO_JOIN_GAME,
	PRO_QUIT_GAME,
	PRO_MAX,
};

class CUserRedis : public CRedis
{
public:
	CUserRedis();
	~CUserRedis();
protected:
	virtual int  GetRedisConfig();
	virtual bool Connected();
	virtual int  Exec(UINT nType,void* pData,UINT nDataSize,void *pRet=NULL,UINT nRet=0);
private:
	void RegRedisScript(int nType,const char* szStr);

	void UserLoginReq(int nUserId,USHORT nSerNo,SERVICEINDEX nSevNo,void* pRet=NULL,UINT nRet=0);
	void UserJoinGame(int nUserId,USHORT nSerNo,SERVICEINDEX nSevNo,int nSeatNo,void* pRet=NULL,UINT nRet=0);
private:
	char* m_RedisPro[PRO_MAX];
};