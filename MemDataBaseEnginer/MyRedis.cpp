#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include "MyRedis.h"
#include "../include/types.h"
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

	if(!Connected())
		return false;

	return true;
}

bool CRedis::OpenConnect()
{
	m_pConn = redisConnect(m_dbConfig.szHost.c_str(), m_dbConfig.nPort);
	
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

	if(m_dbConfig.szAuth.length() > 0)
	{
		redisReply* reply = (redisReply*)redisCommand(m_pConn, "AUTH %s", m_dbConfig.szAuth.c_str());
		if(reply->type == 6)
		{
			printf("auth failer\n");
			freeReplyObject(reply);	
			return false;
		}
		freeReplyObject(reply);	
	}
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
