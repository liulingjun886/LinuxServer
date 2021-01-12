#pragma once

#include <vector>
#include <string>
#include "../Server.h"
#include "../include/ToolLock.h"

class CGameServer : public CServer
{
public:
	CGameServer();
	~CGameServer();
	
public:
	int	 		Initialize();	
	
	uint16 		GetDataSrvIndex(uint32 nRand) const;
	uint16 		GetCenterServerIndex() const;
	uint16 		GetUserServerIndex(uint32 nRand) const;
	

	const char* GetIp() const;
	uint16 		GetPort() const;
	int			GetGameId() const;

	void 		AddConnInfo(uint16 nSerNo,uint16 nIndex);
	uint16  	GetConnSrvIndex(uint16 nSerNo,uint32 nRand);
	void 		DelConnSrvIndex(uint16 nSerNo, uint16 nIndex);

	void		DisconnectToRemoteSrv(uint16 nSrvType, uint16 nSrvNo, uint16 nIndex);
private:
	int  		ReadConfig(const char* szConfigFile);
	int 		ConnectToCenterSrv(const char* szIp, uint16 nPort);
	int 		ConnectToUserSrv(const char* szIp, uint16 nPort);
	int 		ConnectToDataSrv(const char* szIp, uint16 nPort);
	
private:
	CRWLock m_rw_Lock;

	int m_nGameId;

	//对网关开放的地址
	std::string m_szIp;
	uint16 m_nPort;

	//中心服地址
	//std::string m_szCenterIp;
	//unsigned short m_nCenterPort;
	uint16 m_nCenterIndex;

	//用户服务器数据保存
	std::vector<uint16> m_vecUserSerIndex;
	
	//数据服务器数址保存
	std::vector<uint16> m_vecDataSerIndex;

	//存储连接服务器的相关信息
	std::vector<uint16> m_szConnSerInfo[MAX_USER_DATA_SRV_NUM];
};