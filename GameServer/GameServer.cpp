#include "GameServer.h"
#include "../include/IniFile.h"

extern CGameServer* g_pGameServer = NULL;

CGameServer::CGameServer()
{
	g_pGameServer = this;
}

CGameServer::~CGameServer()
{
	
}


int	 CGameServer::Initialize()
{
	return ReadConfig("../config/config.ini");
}

int  CGameServer::ReadConfig(const char* szConfigFile)
{
	IniFile iniFile;
	iniFile.OpenFile(szConfigFile);

	char szFild[64] = {0};
	sprintf(szFild,"game_%d",GetSerNo());
	
	m_szIp = iniFile.ReadString(szFild, "Host", "");
	m_nPort = (USHORT)iniFile.ReadInt(szFild, "Port", 0);
	m_nGameId = iniFile.ReadInt(szFild, "GameID", 0);

	if(0 == m_pCore->AddTcpNetSer(m_szIp.c_str(), m_nPort, false))
		return -1;

	std::string szIp = iniFile.ReadString("centerserver", "Host", "");
	USHORT nPort = iniFile.ReadInt("centerserver", "Port", 0);

	if(0 != ConnectToCenterSrv(szIp.c_str, nPort))
		return -1;
	
	int nUserNum = iniFile.ReadInt("userserver_count", "num", 0);
	if(nUserNum <= 0)
		return -1;

	memset(szFild,sizeof(szFild),0);
	for(int i = 0; i < nUserNum; i++)
	{
		HostConfig conf;
		sprintf(szFild,"userserver_%d",i);
		
		szIp = iniFile.ReadString(szFild, "Host", "");
		nPort = (USHORT)iniFile.ReadInt(szFild, "Port", 0);
		if(0 != ConnectToUserSrv(szIp.c_str(), nPort))
			return -1;
	}
	
	int nDataNum = iniFile.ReadInt("dataserver_count", "num", 0);
	if(nDataNum <= 0)
		return -1;
	memset(szFild,sizeof(szFild),0);
	for(int i = 0; i < nDataNum; i++)
	{
		HostConfig conf;
		sprintf(szFild,"dataserver_%d",i);
		
		szIp = iniFile.ReadString(szFild, "Host", "");
		nPort = (USHORT)iniFile.ReadInt(szFild, "Port", 0);
		if(0 != ConnectToDataSrv(szIp.c_str(), nPort))
			return -1;
	}
	
	iniFile.CloseFile();
	return 0;
}

USHORT 	CGameServer::GetDataSrvIndex(UINT nRand) const
{
	size_t nSize = m_vecDataSerIndex.size();

	if(0 == nSize)
		return 0;

	return m_vecDataSerIndex[nRand%nSize];
}

USHORT 	CGameServer::GetCenterServerIndex() const
{
	return m_nCenterIndex;
}

USHORT 	CGameServer::GetUserServerIndex(UINT nRand) const
{
	size_t nSize = m_vecUserSerIndex.size();
	if(0 == nSize)
		return 0;

	return m_vecUserSerIndex[nRand%nSize];
}


void CGameServer::AddConnInfo(USHORT nSerNo,USHORT nIndex)
{
	std::vector<USHORT>& vecSers = m_szConnSerInfo[nSerNo];
	CToolLock lock(&m_rw_Lock);
	vecSers.push_back(nIndex);
}

USHORT  CGameServer::GetConnSrvIndex(USHORT nSerNo,UINT nRand) const
{
	CToolLock lock(&m_rw_Lock,1);
	
	size_t nSize = m_szConnSerInfo[nSerNo].size();
	if(0 == nSize)
		return 0;
	
	const std::vector<USHORT>& vecSers = m_szConnSerInfo[nSerNo];
	
	return vecSers[nRand%nSize];
}

void CGameServer::DelConnSrvIndex(USHORT nSerNo, USHORT nIndex)
{
	
	size_t nSize = m_szConnSerInfo[nSerNo].size();
	if(0 == nSize)
		return 0;
	
	std::vector<USHORT>& vecSers = m_szConnSerInfo[nSerNo];

	CToolLock lock(&m_rw_Lock,0);
	std::vector<USHORT>::iteretor it = vecSers.begin();
	for(; it != vecSers.end(); it++)
	{
		if(*it == nIndex)
		{
			vecSers.earse(it);
			return;
		}
	}
}

const char* CGameServer::GetIp() const
{
	return m_szIp.c_str();
}
USHORT CGameServer::GetPort() const
{
	return m_nPort;
}

int	CGameServer::GetGameId() const
{
	return m_nGameId;
}

int 	CGameServer::ConnectToCenterSrv(const char* szIp, USHORT nPort)
{
	USHORT nIndex = m_pCore->AddTcpNetCli(szIp, nPort, false);

	if(0 == nIndex)
		return -1;

	m_nCenterIndex = nIndex;
	return 0;
}

int 	CGameServer::ConnectToUserSrv(const char* szIp, USHORT nPort)
{
	USHORT nIndex = m_pCore->AddTcpNetCli(szIp, nPort, false);

	if(0 == nIndex)
		return -1;

	m_vecUserSerIndex.push_back(nIndex);
	return 0;
}

int 	CGameServer::ConnectToDataSrv(const char* szIp, USHORT nPort)
{
	USHORT nIndex = m_pCore->AddTcpNetCli(szIp, nPort, false);

	if(0 == nIndex)
		return -1;

	m_vecDataSerIndex.push_back(nIndex);
	return 0;
}
