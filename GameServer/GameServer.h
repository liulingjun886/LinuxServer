#pragma once

#include "../Server.h"
#include "../include/ToolLock.h"

class CGameServer : public CServer
{
public:
	CGameServer();
	~CGameServer();
	
public:
	int	 		Initialize();	
	
	USHORT 		GetDataSrvIndex(UINT nRand) const;
	USHORT 		GetCenterServerIndex() const;
	USHORT 		GetUserServerIndex(UINT nRand) const;
	

	const char* GetIp() const;
	USHORT 		GetPort() const;
	int			GetGameId() const;

	void 		AddConnInfo(USHORT nSerNo,USHORT nIndex);
	USHORT  	GetConnSrvIndex(USHORT nSerNo,UINT nRand) const;
	void 		DelConnSrvIndex(USHORT nSerNo, USHORT nIndex);

private:
	int  		ReadConfig(const char* szConfigFile);
	int 		ConnectToCenterSrv(const char* szIp, USHORT nPort);
	int 		ConnectToUserSrv(const char* szIp, USHORT nPort);
	int 		ConnectToDataSrv(const char* szIp, USHORT nPort);
	
private:
	CRWLock m_rw_Lock;

	int m_nGameId;

	//对网关开放的地址
	std::string m_szIp;
	USHORT m_nPort;

	//中心服地址
	//std::string m_szCenterIp;
	//unsigned short m_nCenterPort;
	USHORT m_nCenterIndex;

	//用户服务器数据保存
	std::vector<USHORT> m_vecUserSerIndex;
	
	//数据服务器数址保存
	std::vector<USHORT> m_vecDataSerIndex;

	//存储连接服务器的相关信息
	std::vector<USHORT> m_szConnSerInfo[MAX_USER_DATA_SRV_NUM];
};