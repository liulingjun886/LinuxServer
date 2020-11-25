#pragma once
#include "../include/types.h"
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
	
	USHORT s_szConnSer[MAX_CONN_GAME_SRV_NUM];
	USHORT s_szGameSer[MAX_CONN_GAME_SRV_NUM];
	USHORT s_szUserSer[MAX_USER_DATA_SRV_NUM];
	USHORT s_szDataSer[MAX_USER_DATA_SRV_NUM];
	
	std::map<USHORT,USHORT*> m_mapLinkInfo; 	//服务号和游戏服务器的对应
};