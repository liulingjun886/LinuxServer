#include "DataServer.h"
#include "../include/IniFile.h"
#include "../MemDataBaseEnginer/MemDataBaseEnger.h"
#include "../DataBaseEnginer/DataBaseEnginer.h"
#include <stdio.h>
#include "../include/Services.h"
#include "../NetSinkObj.h"
#include <stdio.h>

CDataServer* g_pDataServer = NULL;

CDataServer::CDataServer():m_pMem(NULL)
{
	g_pDataServer = this;
}

CDataServer::~CDataServer()
{
	SAFE_DELTE(m_pMem);
}

int	 CDataServer::Initialize()
{
	char szLogFile[128] = {0};
	sprintf(szLogFile,"DataServer_%d", GetSerNo());
	InitLogFile(szLogFile);

	if(0 != ReadConfig("./config/config.ini"))
		return -1;

	m_pMem = new CMemDataBaseEnginer;
	if(NULL == m_pMem)
		return -1;

	m_pData = new CDataBaseEnginer;
	if(NULL == m_pData)
		return -1;

	m_pMem->SetServiceNum(8);
	m_pData->SetServiceNum(8);

	m_nCenterIndex = m_pCore->AddTcpNetCli(m_szCenterIp.c_str(),m_nCenterPort,false);
	
	if(0 == m_nCenterIndex)
	{
		printf("Connect to center failer\n");
		return -1;
	}

	if(0 == m_pCore->AddTcpNetSer(m_szIp.c_str(), m_nPort, false))
	{
		printf("Add Tcp Server Failer\n");
		return -1;
	}

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
	m_nRedisPort = iniFile.ReadInt(szFild, "redisPort", 0);
	m_szAuth = iniFile.ReadString(szFild, "redisAuth", "");

	m_szDbHost = iniFile.ReadString(szFild, "DBHost", "");
	m_nDbPort = iniFile.ReadInt(szFild, "DBPort", 0);
	m_szDbName = iniFile.ReadString(szFild, "DBName", "");
	m_szDbUserName = iniFile.ReadString(szFild, "DBUserName", "");
	m_szDbPass = iniFile.ReadString(szFild, "DBPassword", "");
	
	//iniFile.CloseFile();
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

SERVICEINDEX CDataServer::GetCenterIndex() const
{
	return m_nCenterIndex;
}


bool CDataServer::PostMemDataBaseReq(CServices* pServices,void* pData, DATASIZE uDataSize)
{
	if(NULL == m_pMem)
	{
		pServices->Log("m_pMem is NULL");
		return false;
	}
	
	SERVICEINDEX nIndex = m_pMem->GetIndex(pServices->GetServiceIndex());
	if(nIndex == INVALID_SERIVCE_INDEX)
	{
		pServices->Log("No memdatabase services");
		return false;
	}
	return pServices->PostData(nIndex, MEM_DATA_BASE_REQ, pData, uDataSize);
}

bool CDataServer::PostMemDataBaseRet(CServices* pServices,SERVICEINDEX nToSerId,SERVICEINDEX nCsid, uint32 nTypeId, void* pData, DATASIZE nDataSize)
{
	static DATASIZE nHeadSize = sizeof(DataCenter) + sizeof(uint32);
	char* buff[MAX_MSG_SIZE] = {0};
	DataCenter* pCenter = (DataCenter*)buff;
	pCenter->nCsid = nCsid;
	uint32* pType = (uint32*)(pCenter+1);
	*pType = nTypeId;
	if (nDataSize > 0)
	{
		memcpy(pType+1, pData, nDataSize);
	}
	CNetSinkObj::SendData(pServices,  nToSerId, MAIN_MSG_DATASER, DS_SUB_MSG_MEM_BASE_RET, buff, nHeadSize + nDataSize);
	return true;
}

bool CDataServer::PostDataBaseReq(CServices* pServices,void* pData, DATASIZE uDataSize)
{
	if(NULL == m_pData)
	{
		pServices->Log("no databaseEnginer");
		return false;
	}
	
	SERVICEINDEX nIndex = m_pData->GetIndex(pServices->GetServiceIndex());
	if(nIndex == INVALID_SERIVCE_INDEX)
	{
		pServices->Log("no databaseService");
		return false;
	}

	return pServices->PostData(nIndex, DATA_BASE_REQ, pData, uDataSize);
}

bool CDataServer::PostDataBaseRet(CServices* pServices,SERVICEINDEX nToSerId,SERVICEINDEX nCsid, uint32 uTypeId, void* pData, DATASIZE nDataSize)
{
	static DATASIZE nHeadSize = sizeof(DataCenter) + sizeof(uint32);
	char* buff[MAX_MSG_SIZE] = {0};
	DataCenter* pCenter = (DataCenter*)buff;
	pCenter->nCsid = nCsid;
	uint32* pType = (uint32*)(pCenter+1);
	*pType = uTypeId;
	if (nDataSize > 0)
	{
		memcpy(pType+1, pData, nDataSize);
	}
	CNetSinkObj::SendData(pServices,  nToSerId, MAIN_MSG_DATASER,DS_SUB_MSG_DATA_BASE_RET , buff, nHeadSize + nDataSize);
	return true;
}


