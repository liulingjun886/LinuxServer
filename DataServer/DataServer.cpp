#include "DataServer.h"
#include "../include/IniFile.h"

CDataServer::CDataServer(CServices* pServices):CNetHandSink(pServices)
{
	
}

CDataServer::~CDataServer()
{
	
}

int	 CDataServer::Initialize()
{
	if(0 != ReadConfig("../config/config.ini"))
		return -1;

	m_pMem = Single_Create(CMemDataBaseEnginer);
	if(NULL == m_pMem)
		return -1;

	m_pData = Single_Create(CDataBaseEnginer);
	if(NULL == m_pData)
		return -1;

	m_pMem->SetServiceNum(8);
	m_pData->SetServiceNum(8);

	if(0 == m_pCore->AddTcpNetCli(m_szCenterIp.c_str(),m_nCenterPort,false))
		return -1

	if(0 == m_pCore->AddTcpNetSer(m_szIp.c_str(), m_nPort, false))
		return -1;

	return 0;	
}

int  CDataServer::ReadConfig(const char* szConfigFile)
{
	IniFile iniFile;
	iniFile.OpenFile(szConfigFile);

	m_szCenterIp = iniFile.ReadString("centerserver", "Host", "");
	m_nCenterPort = iniFile.ReadInt("centerserver", "Port", 0);

	char szFild[64] = {0};
	sprintf(szFild,"dataserver_%d",GetSerNo());
	m_szIp = iniFile.ReadString(szFild, "Host", "");
	m_nPort = iniFile.ReadInt(szFild, "Port", 0);

	m_szRedisHost = iniFile.ReadString(szFild, "redisHost", "");
	m_nRedisPort = iniFile.ReadInt(szFild, "redisPort", 0;
	m_szAuth = iniFile.ReadString(szFild, "redisAuth", "");

	m_szDbHost = iniFile.ReadString(szFild, "DBHost", "");
	m_nDbPort = iniFile.ReadInt(szFild, "DBPort", 0;
	m_szDbName = iniFile.ReadString(szFild, "DBName", "");
	m_szDbUserName = iniFile.ReadString(szFild, "DBUserName", "");
	m_szDbPass = iniFile.ReadString(szFild, "DBPassword", "");
	
	iniFile.CloseFile();
	return 0;
}

const char* CDataServer::GetRedisHost() const
{
	return m_szRedisHost.c_str();
}

const unsigned short CDataServer::GetRedisPort() const
{
	return m_nRedisPort;
}

const char* CDataServer::GetRedisAuth() const
{
	return m_szAuth.c_str();
}

const char* CDataServer::GetDbHost() const
{
	return m_szDbHost.c_str();
}

const unsigned short CDataServer::GetDbPort() const
{
	return m_nDbPort;
}

const char* CDataServer::GetDbName() const
{
	return m_szDbName.c_str();
}

const char* CDataServer::GetDbUserName() const
{
	return m_szDbUserName.c_str();
}

const char* CDataServer::GetDbPass() const
{
	return m_szDbPass.c_str();
}


