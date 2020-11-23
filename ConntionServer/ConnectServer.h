#pragma once

#include "../Server.h"

#include <vector>
#include <map>
#include <string>

struct HostConfig
{
	std::string szIp;
	USHORT nPort;
};

struct GameInfo : public HostConfig
{
	int 	nGameId;
	int 	nGameNo;
	
};

class CConnectServer : public CServer
{
public:
	CConnectServer();
	~CConnectServer();
public:
	int	 Initialize();	
	int	 ConnectToGameServer(const GameInfo& info);
	USHORT  GetGameSerIndexByNo(USHORT nGameSrvNo);
	USHORT 	GetGameSerNoByGameId(int nGameId);
	USHORT 	GetARandGameSer();
	USHORT 	GetCenterServerIndex() const;
	USHORT 	GetUserServerIndex(UINT nRand) const;
private:
	int  ReadConfig(const char* szConfigFile);
private:
	//std::vector<GameInfo> m_vecConfigGameInfo;			//存储游戏服相关配置
	std::vector<USHORT> m_vecGameConnIndex[MAX_CONN_GAME_SRV_NUM];			//存储游戏服连接按游戏服serid
	std::map<int,std::vector<USHORT> > m_mapGameSerInfo;	//存储游戏关连的游戏服

	//对客户端开发的地址
	std::string m_szIp;
	USHORT m_nPort;

	//中心服地址
	std::string m_szCenterIp;
	unsigned short m_nCenterPort;
	USHORT m_nCenterIndex;

	//用户服务器数据保存
	std::vector<HostConfig> m_vecUserSerConfig;
	std::vector<USHORT> m_vecUserSerIndex;
};