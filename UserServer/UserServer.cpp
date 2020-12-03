#include "UserServer.h"
#include "../include/IniFile.h"
#include "../include/Services.h"
#include "../NetSinkObj.h"
#include "../MemDataBaseEnginer/MemDataBaseEnger.h"
#include <stdio.h>

CUserServer* g_pUserServer = NULL;


CUserServer::CUserServer():m_pMem(NULL)
{
	g_pUserServer = this;
}

CUserServer::~CUserServer()
{
	SAFE_DELTE(m_pMem);
}

int	CUserServer::Initialize()
{
	InitLogFile("UserServer");

	if(0 != ReadConfig("./config/config.ini"))
	{
		printf("readconfig failer!\n");
		return -1;
	}

	m_pMem = new CMemDataBaseEnginer;
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

bool CUserServer::PostMemDataBaseReq(CServices* pServices,void* pData, DATASIZE uDataSize)
{
	SERVICEINDEX nIndex = m_pMem->GetIndex(pServices->GetServiceIndex());
	if(nIndex == INVALID_SERIVCE_INDEX)
		return false;
	return pServices->PostData(nIndex, MEM_DATA_BASE_REQ, pData, uDataSize);
}

bool CUserServer::PostMemDataBaseRet(CServices* pServices,SERVICEINDEX nToSerId,SERVICEINDEX nCsid, uint32 nTypeId, void* pData, DATASIZE nDataSize)
{
	/*static DATASIZE nHeadSize = sizeof(DataCenter) + sizeof(uint32);
	char* buff[MAX_MSG_SIZE] = {0};
	DataCenter* pCenter = (DataCenter*)buff;
	pCenter->nCsid = nCsid;
	uint32* pType = (uint32*)(pCenter+1);
	*pType = nTypeId;
	if (nDataSize > 0)
	{
		memcpy(pType+1, pData, nDataSize);
	}
	CNetSinkObj::SendData(pServices,  nToSerId, MAIN_MSG_USERSER, US_SUB_MSG_MEM_BASE_RET,buff, nHeadSize + nDataSize);*/
	return true;
}

