#include "GameServer.h"
#include "../include/IniFile.h"
#include <algorithm>
#include <stdio.h>
#include "Room.h"
#include "GameUserManager.h"

#define MAX_USER_SERICES_NUM 0XFF

CGameServer* g_pGameServer = NULL;

CGameServer::CGameServer()
{
	g_pGameServer = this;
}

CGameServer::~CGameServer()
{
	CRoom::ReleaseGameLogic();
}


int	 CGameServer::Initialize()
{
	char szLogFile[128] = {0};
	sprintf(szLogFile,"GameServer_%d",GetSerNo());
	InitLogFile(szLogFile);
	
	return ReadConfig("./config/config.ini");
}

int  CGameServer::ReadConfig(const char* szConfigFile)
{
	IniFile iniFile;
	iniFile.OpenFile(szConfigFile);

	char szFild[64] = {0};
	sprintf(szFild,"game_%d",GetSerNo());
	
	m_szIp = iniFile.ReadString(szFild, "Host", "");
	m_nPort = (uint16)iniFile.ReadInt(szFild, "Port", 0);
	m_nGameId = iniFile.ReadInt(szFild, "GameID", 0);

	if(!CRoom::LoadGameLogic(m_nGameId))
	{
		printf("LoadGameLogic Failer\n");
		return -1;
	}

	if(0 == m_pCore->AddTcpNetSer(m_szIp.c_str(), m_nPort, false))
		return -1;

	std::string szIp = iniFile.ReadString("centerserver", "Host", "");
	uint16 nPort = iniFile.ReadInt("centerserver", "Port", 0);

	if(0 != ConnectToCenterSrv(szIp.c_str(), nPort))
		return -1;
	
	int nUserNum = iniFile.ReadInt("userserver_count", "num", 0);
	if(nUserNum <= 0)
		return -1;

	memset(szFild,0,sizeof(szFild));
	for(int i = 0; i < nUserNum; i++)
	{
		HostConfig conf;
		sprintf(szFild,"userserver_%d",i);
		
		szIp = iniFile.ReadString(szFild, "Host", "");
		nPort = (uint16)iniFile.ReadInt(szFild, "Port", 0);
		if(0 != ConnectToUserSrv(szIp.c_str(), nPort))
			return -1;
	}
	
	int nDataNum = iniFile.ReadInt("dataserver_count", "num", 0);
	if(nDataNum <= 0)
		return -1;
	
	memset(szFild,0,sizeof(szFild));
	for(int i = 0; i < nDataNum; i++)
	{
		HostConfig conf;
		sprintf(szFild,"dataserver_%d",i);
		
		szIp = iniFile.ReadString(szFild, "Host", "");
		nPort = (uint16)iniFile.ReadInt(szFild, "Port", 0);
		if(0 != ConnectToDataSrv(szIp.c_str(), nPort))
			return -1;
	}

	for(uint32 i = 0; i < MAX_USER_SERICES_NUM; i++)
	{
		CGameUserManager* pUserManager = new CGameUserManager;
		if(!Single_Get(CCore)->AddService(pUserManager))
			return -1;
		m_vecUserManager.push_back(pUserManager->GetServiceIndex());
	}
	
	return 0;
}

uint16 	CGameServer::GetDataSrvIndex(uint32 nRand) const
{
	size_t nSize = m_vecDataSerIndex.size();

	if(0 == nSize)
		return 0;

	return m_vecDataSerIndex[nRand%nSize];
}

uint16 	CGameServer::GetCenterServerIndex() const
{
	return m_nCenterIndex;
}

uint16 	CGameServer::GetUserServerIndex(uint32 nRand) const
{
	size_t nSize = m_vecUserSerIndex.size();
	if(0 == nSize)
		return 0;

	return m_vecUserSerIndex[nRand%nSize];
}


void CGameServer::AddConnInfo(uint16 nSerNo,uint16 nIndex)
{
	std::vector<uint16>& vecSers = m_szConnSerInfo[nSerNo];
	
	CToolLock lock(&m_rw_Lock);
	vecSers.push_back(nIndex);
}

uint16  CGameServer::GetConnSrvIndex(uint16 nSerNo,uint32 nRand)
{
	CToolLock lock(&m_rw_Lock,1);
	
	size_t nSize = m_szConnSerInfo[nSerNo].size();
	if(0 == nSize)
		return 0;
	
	const std::vector<uint16>& vecSers = m_szConnSerInfo[nSerNo];
	
	return vecSers[nRand%nSize];
}

void CGameServer::DelConnSrvIndex(uint16 nSerNo, uint16 nIndex)
{

	std::vector<uint16>& vecSers = m_szConnSerInfo[nSerNo];
	size_t nSize = vecSers.size();
	if(0 == nSize)
		return;
	
	CToolLock lock(&m_rw_Lock,0);
	std::vector<uint16>::iterator it = vecSers.begin();
	for(; it != vecSers.end(); it++)
	{
		if(*it == nIndex)
		{
			vecSers.erase(it);
			return;
		}
	}
}

void CGameServer::DisconnectToRemoteSrv(uint16 nSrvType, uint16 nSrvNo, uint16 nIndex)
{
	switch(nSrvType)
	{
		case SRV_TYPE_CENTER:
		{
			if(m_nCenterIndex == nIndex)
				m_nCenterIndex = 0;
			break;
		}
		case SRV_TYPE_USER:
		{
			std::vector<uint16>::iterator it = find(m_vecUserSerIndex.begin(), m_vecUserSerIndex.end(),nIndex);
			if(it != m_vecUserSerIndex.end())
			{
				m_vecUserSerIndex.erase(it);
			}
			break;
		}
		case SRV_TYPE_DATA:
		{
			std::vector<uint16>::iterator it = find(m_vecDataSerIndex.begin(), m_vecDataSerIndex.end(),nIndex);
			if(it != m_vecDataSerIndex.end())
			{
				m_vecDataSerIndex.erase(it);
			}
			break;
		}
		default:
			break;
	}
}


const char* CGameServer::GetIp() const
{
	return m_szIp.c_str();
}

uint16 CGameServer::GetPort() const
{
	return m_nPort;
}

int	CGameServer::GetGameId() const
{
	return m_nGameId;
}

int CGameServer::ConnectToCenterSrv(const char* szIp, uint16 nPort)
{
	uint16 nIndex = m_pCore->AddTcpNetCli(szIp, nPort, false);

	if(0 == nIndex)
		return -1;

	m_nCenterIndex = nIndex;
	return 0;
}

int CGameServer::ConnectToUserSrv(const char* szIp, uint16 nPort)
{
	uint16 nIndex = m_pCore->AddTcpNetCli(szIp, nPort, false);

	if(0 == nIndex)
		return -1;

	m_vecUserSerIndex.push_back(nIndex);
	return 0;
}

int CGameServer::ConnectToDataSrv(const char* szIp, uint16 nPort)
{
	uint16 nIndex = m_pCore->AddTcpNetCli(szIp, nPort, false);

	if(0 == nIndex)
		return -1;

	m_vecDataSerIndex.push_back(nIndex);
	return 0;
}

SERVICEINDEX CGameServer::GetUserServiceIndex(UID nUid)
{
	uint32 nSize = (uint32)m_vecUserManager.size();
	if(0 == nSize)
		return INVALID_SERIVCE_INDEX;

	return m_vecUserManager[nUid%nSize];
}

