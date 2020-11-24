#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include "MyRedis.h"
#include "types.h"
#include "MemDataDef.h"
#include "MemDataDef.h"

using namespace std;
using namespace Mem;

CRedis::CRedis() : m_pConn(NULL),m_pReply(NULL)
{
	
}

CRedis::~CRedis()
{
	CloseConnect();
}

bool CRedis::InitConnection()
{
	if(0 != GetRedisConfig())
		return false;

	if(!OpenConnect())
		return false;

	if(0 != Connected())
		return false;

	return true;
}

bool CRedis::OpenConnect()
{
	m_pConn = redisConnect(m_dbConfig.szHost, m_dbConfig.nPort);
	
	if(!m_pConn)
	{
		printf("redis connect refuse\n");
		return false;
	}
	
	if(m_pConn->err != 0)
	{
		printf("redis connect failer:%s\n",m_pConn->errstr);
		return false;
	}
	
	redisReply* reply = (redisReply*)redisCommand(m_pConn, "AUTH %s", m_dbConfig.szAuth);
	if(reply->type == 6)
	{
		printf("auth failer\n");
		return false;
	}
	
	freeReplyObject(reply);	
	return true;
}

void CRedis::CloseConnect()
{	
	if(m_pConn)
	{
		redisFree(m_pConn);
		m_pConn = NULL;
	}
}

void CRedis::CloseReply()
{
	if(m_pReply)
	{
		freeReplyObject(m_pReply);
		m_pReply = NULL;
	}
}
