#pragma once
#include "../Server.h"
#include <string>
#include <map>


class CCenterServer : public CServer
{
public:
	CCenterServer();
	~CCenterServer();
public:
	int	 Initialize();
private:
	int  ReadConfig(const char* szConfigFile);
private:
	std::string m_szIp;
	unsigned short m_nPort;
public:
	std::map<USHORT,GameInfo> m_mapGameInfo; //游戏服务器号和游戏信息的对应
	
	USHORT s_szConnSer[500];
	USHORT s_szGameSer[500];
	USHORT s_szUserSer[16];
	USHORT s_szDataSer[16];
	
	std::map<USHORT,USHORT*> m_mapLinkInfo; //服务号和游戏服务器的对应
};