#pragma once

#include "../MemDataBaseEnginer/MyRedis.h"

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
	CUserRedis(CServices* pService);
	~CUserRedis();
protected:
	virtual int  GetRedisConfig();
	virtual bool Connected();
	virtual int  Exec(SERVICEINDEX nSrcIndex,SERVICEINDEX nCsid,uint32 nType,void* pData,DATASIZE nDataSize);
private:
	void RegRedisScript(int nType,const char* szStr);

	void UserLoginReq(SERVICEINDEX nSrcIndex,SERVICEINDEX nCsid,int nUserId,uint16 nSerNo,SERVICEINDEX nSevNo);
	void UserJoinGame(SERVICEINDEX nSrcIndex,SERVICEINDEX nCsid,int nUserId,uint16 nSerNo,SERVICEINDEX nSevNo,int nSeatNo);
private:
	char* m_RedisPro[PRO_MAX];
};