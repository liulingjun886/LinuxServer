#include "UserServer.h"
#include "../include/core/IniFile.h"
#include "../include/core/Services.h"
#include "../NetSinkObj.h"
#include "../MemDataBaseEnginer/MemDataBaseEnger.h"
#include <stdio.h>
#include "UserCliSink.h"
#include "UserSerSink.h"

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
	char szLogFile[128] = {0};
	sprintf(szLogFile,"UserServer_%d", GetSerNo());
	InitLogFile(szLogFile);

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
	
	if(0 == m_pCore->AddTcpNetCli(m_szCenterIp.c_str(),m_nCenterPort, CreateNetSink<CUserCliSink>, false))
	{
		printf("connect to center Failer!\n");
		return -1;
	}
		

	if(0 == m_pCore->AddTcpNetSer(m_szIp.c_str(), m_nPort, CreateNetSink<CUserSerSink>, false))
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
	COutputPacket out;
	out.Begin(MAIN_MSG_USERSER,US_SUB_MSG_MEM_BASE_RET);
	out.WriteInt16(nCsid);
	out.WriteInt32(nTypeId);
	out.WriteBinary(pData, nDataSize);
	out.End();
	CNetSinkObj::SendData(pServices, nToSerId, out);
	return true;
}

