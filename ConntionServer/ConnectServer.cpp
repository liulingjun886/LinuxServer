#include "ConnectServer.h"
#include "../include/IniFile.h"
#include <algorithm>
#include <stdio.h>

CConnectServer* g_pConnectServer = NULL;

CConnectServer::CConnectServer()
{
	g_pConnectServer = this;
	//m_vecConfigGameInfo.clear();
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
	char szLogFile[128] = {0};
	sprintf(szLogFile,"ConnServer_%d",GetSerNo());
	InitLogFile(szLogFile);
	
	if(0 != ReadConfig("./config/config.ini"))
		return -1;

	m_nCenterIndex = m_pCore->AddTcpNetCli(m_szCenterIp.c_str(), m_nCenterPort, false);
	if(0 == m_nCenterIndex)
		return -1;

	std::vector<HostConfig>::iterator it = m_vecUserSerConfig.begin();
	for(; it != m_vecUserSerConfig.end(); it++)
	{
		uint16 nIndex = m_pCore->AddTcpNetCli(it->szIp.c_str(), it->nPort, false);
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

	memset(szFild, 0, sizeof(szFild));
	for(int i = 0; i < nNum; i++)
	{
		HostConfig conf;
		sprintf(szFild,"userserver_%d",i);
		conf.szIp = iniFile.ReadString(szFild, "Host", "");
		conf.nPort = iniFile.ReadInt(szFild, "Port", 0);
		m_vecUserSerConfig.push_back(conf);
	}
	
	
	//iniFile.CloseFile();
	return 0;
}

int	 CConnectServer::ConnectToGameServer(const GameInfo& info)
{
	std::vector<uint16>::iterator it = m_vecGameConnIndex[info.nGameNo].begin();
	while(it != m_vecGameConnIndex[info.nGameNo].end())
	{
		
	}

	for(int i = 0; i < MAX_CONN_TO_GAME_LINK; i++)
	{
		uint16 nIndex = m_pCore->AddTcpNetCli(info.szIp.c_str(), info.nPort, false);
		if(0 == nIndex)
			return -1;
		
		m_vecGameConnIndex[info.nGameNo].push_back(nIndex);
	}
	m_mapGameSerInfo[info.nGameId].push_back(info.nGameNo);
	m_mapGameNoToGameId[info.nGameNo] = info.nGameId;
	
	return 0;
}


int  CConnectServer::DisconnectToServer(uint16 nSerType, uint16 nSerNo, uint16 nIndex)
{
	switch(nSerType)
	{
		case SRV_TYPE_CENTER:
		{
			if(m_nCenterIndex == nIndex)
				m_nCenterIndex = 0;
			return 0;
		}
		case SRV_TYPE_USER:
		{
			std::vector<uint16>::iterator it = find(m_vecUserSerIndex.begin(),m_vecUserSerIndex.end(),nIndex);
			if(it != m_vecUserSerIndex.end())
				m_vecUserSerIndex.erase(it);
			return 0;
		}
		case SRV_TYPE_GAME:
		{
			std::vector<uint16>& vecSerNo = m_vecGameConnIndex[nSerNo];
			std::vector<uint16>::iterator it = find(vecSerNo.begin(),vecSerNo.end(),nIndex);
			if(it == vecSerNo.end())
				return 0;
			
			vecSerNo.erase(it);
			if(0 == vecSerNo.size())
			{
				std::map<uint16, int>::iterator it_ser_game = m_mapGameNoToGameId.find(nSerNo);
				if(it_ser_game == m_mapGameNoToGameId.end())
					return 0;
				
				std::map<int,std::vector<uint16> >::iterator it_game_ser = m_mapGameSerInfo.find(it_ser_game->second);
				std::vector<uint16>& vecGameSers = it_game_ser->second;
				std::vector<uint16>::iterator it_ser = find(vecGameSers.begin(),vecGameSers.end(),nSerNo);
				if(it_ser == vecGameSers.end())
					return 0;

				vecGameSers.erase(it_ser);
				m_mapGameNoToGameId.erase(it_ser_game);
			}
			return 0;
		}
	}
	return 0;
}



uint16  CConnectServer::GetGameSerIndexByNo(uint16 nGameSrvNo,uint32 nRand)
{
	if(0 == m_vecGameConnIndex[nGameSrvNo].size())
		return 0;

	int nIndex = nRand % m_vecGameConnIndex[nGameSrvNo].size();
	return m_vecGameConnIndex[nGameSrvNo][nIndex];
}

uint16 	CConnectServer::GetGameSerNoByGameId(int nGameId)
{
	std::map<int,std::vector<uint16> >::iterator it = m_mapGameSerInfo.find(nGameId);
	if(it == m_mapGameSerInfo.end())
		return 0;

	const std::vector<uint16>& vecGameInfo = it->second;

	if(0 == vecGameInfo.size())
		return 0;
	
	return vecGameInfo[rand()%vecGameInfo.size()];
}

uint16 	CConnectServer::GetARandGameSer()
{
	std::map<int,std::vector<uint16> >::iterator it = m_mapGameSerInfo.begin();
	if(it == m_mapGameSerInfo.end())
		return 0;

	const std::vector<uint16>& vecSerNos = it->second;
	return vecSerNos[0];
}


uint16 	CConnectServer::GetCenterServerIndex() const
{
	return m_nCenterIndex;
}

uint16 	CConnectServer::GetUserServerIndex(uint32 nRand) const
{
	size_t nSize = m_vecUserSerIndex.size();
	return m_vecUserSerIndex[(nRand % nSize)];
}

