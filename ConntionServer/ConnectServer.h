#pragma once

#include "../Server.h"

#include <vector>
#include <map>
#include <string>

class CConnectServer : public CServer
{
public:
	CConnectServer();
	~CConnectServer();
public:
	int	 Initialize();	
	int	 ConnectToGameServer(const GameInfo& info);
	int  DisconnectToServer(uint16 nSerType, uint16 nSerNo, uint16 nIndex);
	
	uint16  GetGameSerIndexByNo(uint16 nGameSrvNo,uint32 nRand);
	uint16 	GetGameSerNoByGameId(int nGameId);
	uint16 	GetARandGameSer();
	uint16 	GetCenterServerIndex() const;
	uint16 	GetUserServerIndex(uint32 nRand) const;
private:
	int  ReadConfig(const char* szConfigFile);
private:
	//std::vector<GameInfo> m_vecConfigGameInfo;			//存储游戏服相关配置
	std::vector<uint16> m_vecGameConnIndex[MAX_CONN_GAME_SRV_NUM];//存储游戏服连接按游戏服serid
	std::map<int,std::vector<uint16> > m_mapGameSerInfo;	//GameId 与 游戏服对应
	std::map<uint16, int> m_mapGameNoToGameId;				//游戏服与GameId对应

	//对客户端开发的地址
	std::string m_szIp;
	uint16 m_nPort;

	//中心服地址
	std::string m_szCenterIp;
	unsigned short m_nCenterPort;
	uint16 m_nCenterIndex;

	//用户服务器数据保存
	std::vector<HostConfig> m_vecUserSerConfig;
	std::vector<uint16> m_vecUserSerIndex;
};