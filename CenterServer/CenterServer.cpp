#include "CenterServer.h"
#include "../include/core/IniFile.h"
#include <stdio.h>


CCenterServer* g_pCenterServer = NULL;


CCenterServer::CCenterServer()
{
	g_pCenterServer = this;
	
	m_mapGameInfo.clear();
	
	memset(s_szConnSer,0,sizeof(s_szConnSer));
	memset(s_szGameSer,0,sizeof(s_szGameSer));
	memset(s_szUserSer,0,sizeof(s_szUserSer));
	memset(s_szDataSer,0,sizeof(s_szDataSer));

	m_mapLinkInfo.clear();
}

CCenterServer::~CCenterServer()
{
	m_mapGameInfo.clear();
	m_mapLinkInfo.clear();
}

int  CCenterServer::ReadConfig(const char* szConfigFile)
{
	IniFile iniFile;
	iniFile.OpenFile(szConfigFile);

	m_szIp = iniFile.ReadString("centerserver", "Host", "");
	m_nPort = iniFile.ReadInt("centerserver", "Port", 0);

	//iniFile.CloseFile();
	return 0;
}

int	 CCenterServer::Initialize()
{
	if(0 != ReadConfig("./config/config.ini"))
	{
		printf("readconfig failer!\n");
		return -1;
	}
		
	
	if(0 == m_pCore->AddTcpNetSer(m_szIp.c_str(), m_nPort, false))
	{
		printf("AddTcpNetSer Failer!\n");
		return -1;
	}
		
	
	InitLogFile("CenterServer");
	return 0;
}

