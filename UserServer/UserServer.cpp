#include "UserServer.h"
#include "../include/IniFile.h"
#include "../MemDataBaseEnginer/MemDataBaseEnger.h"
#include <stdio.h>

CUserServer* g_pUserServer = NULL;


CUserServer::CUserServer():m_pMem(NULL)
{
	g_pUserServer = this;
}
CUserServer::~CUserServer()
{
	
}

int	CUserServer::Initialize()
{
	if(0 != ReadConfig("./config/config.ini"))
	{
		printf("readconfig failer!\n");
		return -1;
	}

	m_pMem = Single_Create(CMemDataBaseEnginer);
	if(NULL == m_pMem)
	{
		printf("CMemDataBaseEnginer create failer!\n");
		return -1;
	}
	m_pMem->SetServiceNum(8);
	
	if(0 == m_pCore->AddTcpNetCli(m_szCenterIp.c_str(),m_nCenterPort,false))
	{
		printf("connect to center Failer!\n");
		return -1;
	}
		

	if(0 == m_pCore->AddTcpNetSer(m_szIp.c_str(), m_nPort, false))
	{
		printf("AddTcpNetSer Failer!\n");
		return -1;
	}
		

	InitLogFile("UserServer");
	return 0;
}

int CUserServer::ReadConfig(const char* szConfigFile)
{
	IniFile iniFile;
	iniFile.OpenFile(szConfigFile);

	m_szCenterIp = iniFile.ReadString("centerserver", "Host", "");
	m_nCenterPort = iniFile.ReadInt("centerserver", "Port", 0);

	char szFild[64] = {0};
	sprintf(szFild,"userserver_%d",GetSerNo());
	m_szIp = iniFile.ReadString(szFild, "Host", "");
	m_nPort = iniFile.ReadInt(szFild, "Port", 0);

	m_szRedisHost = iniFile.ReadString(szFild, "redisHost", "");
	m_nRedisPort = iniFile.ReadInt(szFild, "redisPort", 0);
	m_szAuth = iniFile.ReadString(szFild, "redisAuth", "");
	
	return 0;
}


const char* CUserServer::GetRedisHost() const
{
	return m_szRedisHost.c_str();
}

const unsigned short CUserServer::GetRedisPort() const
{
	return m_nRedisPort;
}

const char* CUserServer::GetRedisAuth() const
{
	return m_szAuth.c_str();
}

