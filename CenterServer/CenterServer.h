#pragma once
#include "../Defines.h"
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
	std::map<uint16,GameInfo> m_mapGameInfo; //游戏服务器号和游戏信息的对应
	
	uint16 s_szConnSer[MAX_CONN_GAME_SRV_NUM];
	uint16 s_szGameSer[MAX_CONN_GAME_SRV_NUM];
	uint16 s_szUserSer[MAX_USER_DATA_SRV_NUM];
	uint16 s_szDataSer[MAX_USER_DATA_SRV_NUM];
	
	std::map<uint16,uint16*> m_mapLinkInfo; 	//服务号和游戏服务器的对应
};