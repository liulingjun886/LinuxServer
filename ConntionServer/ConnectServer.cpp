#include "ConnectServer.h"
#include "../include/IniFile.h"

CConnectServer* g_pConnectServer = NULL;

CConnectServer::CConnectServer()
{
	g_pConnectServer = this;
	m_vecConfigGameInfo.clear();
	m_mapGameSerInfo.clear();
	m_vecUserSerConfig.clear();
	m_vecUserSerIndex.clear();

	for(int i = 0; i < 500; i++)
	{
		m_vecGameConnIndex[i].clear();
	}
}

CConnectServer::~CConnectServer()
{
	
}

int	 CConnectServer::Initialize()
{
	if(0 != ReadConfig("../config/config.ini"))
		return -1;

	m_nCenterIndex = m_pCore->AddTcpNetCli(m_szCenterIp.c_str(), m_nCenterPort, false);
	if(0 == m_nCenterIndex)
		return -1;

	std::vector<HostConfig>::iteretor it = m_vecUserSerConfig.begin();
	for(; it != m_vecUserSerConfig.end(); it++)
	{
		USHORT nIndex = m_pCore->AddTcpNetCli(it->szIp.c_str(), it->nPort, false);
		if(0 == nIndex)
			return -1;
		m_vecUserSerIndex.push_back(nIndex);
	}

	if(0 == m_pCore->AddTcpNetSer(m_szIp.c_str(), m_nPort, false))
		return -1;
	
	return 0;
}

int  CConnectServer::ReadConfig(const char* szConfigFile)
{
	IniFile iniFile;
	iniFile.OpenFile(szConfigFile);

	m_szCenterIp = iniFile.ReadString("centerserver", "Host", "");
	m_nCenterPort = iniFile.ReadInt("centerserver", "Port", 0);

	char szFild[64] = {0};
	sprintf(szFild,"conn_%d",GetSerNo());
	m_szIp = iniFile.ReadString(szFild, "Host", "");
	m_nPort = iniFile.ReadInt(szFild, "Port", 0);

	int nNum = iniFile.ReadInt("userserver_count", "num", 0);
	if(nNum <= 0)
		return -1;

	memset(szFild,sizeof(szFild),0);
	for(int i = 0; i < nNum; i++)
	{
		HostConfig conf;
		sprintf(szFild,"userserver_%d",i);
		conf.szIp = iniFile.ReadString(szFild, "Host", "");
		conf.nPort = iniFile.ReadInt(szFild, "Port", 0);
		m_vecUserSerConfig.push_back(conf);
	}
	
	
	iniFile.CloseFile();
	return 0;
}

int	 CConnectServer::ConnectToGameServer(const GameInfo& info)
{
	for(int i = 0; i < 31; i++)
	{
		USHORT nIndex = m_pCore->AddTcpNetCli(info.szIp.c_str(), info.nPort, false);
		if(0 == nIndex)
			return -1;
		
		m_vecGameConnIndex[info.nGameNo].push_back(nIndex);
	}
	m_mapGameSerInfo[info.nGameId].push_back(info.nGameNo);
	return 0;
}


USHORT  CConnectServer::GetGameSerIndexByNo(USHORT nGameSrvNo,UINT nRand)
{
	if(0 == m_vecGameConnIndex[nGameSrvNo].size())
		return 0;

	int nIndex = nRand % m_vecGameConnIndex[nGameSrvNo].size();
	return m_vecGameConnIndex[nGameSrvNo][nIndex];
}

USHORT 	CConnectServer::GetGameSerNoByGameId(int nGameId)
{
	std::map<int,std::vector<USHORT> >::iteretor it = m_mapGameSerInfo.find(nGameId);
	if(it == m_mapGameSerInfo.end())
		return 0;

	const std::vector<USHORT>& vecGameInfo = it->second;

	if(0 == vecGameInfo.size())
		return 0;
	
	return vecGameInfo[rand()%vecGameInfo.size()];
}

USHORT 	CConnectServer::GetARandGameSer()
{
	std::map<int,std::vector<USHORT> >::iteretor it = m_mapGameSerInfo.begin();
	if(it == m_mapGameSerInfo.end())
		return 0;

	const std::vector<USHORT>& vecSerNos = it->second;
	return vecSerNos[0];
}


USHORT 	CConnectServer::GetCenterServerIndex() const
{
	return m_nCenterIndex;
}

USHORT 	CConnectServer::GetUserServerIndex(UINT nRand) const
{
	size_t nSize = m_vecUserSerIndex.size();
	return m_vecUserSerIndex[(nRand % nSize)];
}

