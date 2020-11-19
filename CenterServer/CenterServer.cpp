#include "CenterServer.h"
#include "IniFile.h"

extern unsigned short g_nSerType;
extern unsigned short g_nSerNo;

CCenterServer::CCenterServer()
{
	
}

CCenterServer::~CCenterServer()
{
	
}

int  CCenterServer::ReadConfig(const char* szConfigFile)
{
	IniFile iniFile;
	iniFile.OpenFile(szConfigFile);

	m_szIp = iniFile.ReadString("centerserver", "Host", "");
	m_nPort = iniFile.ReadInt("centerserver", "Port", 0);

	iniFile.CloseFile();
	return 0;
}

int	 CCenterServer::Initialize()
{
	if(0 != ReadConfig("./config/config.ini"))
		return -1;
	
	if(0 == m_pCore->AddTcpNetSer(m_szIp.c_str(), m_nPort, false))
		return -1;
	
	InitLogFile("CenterServer");
	return 0;
}

