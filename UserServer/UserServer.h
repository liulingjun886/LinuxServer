#pragma once
#include "../Server.h"
#include <string>

class CMemDataBaseEnginer;

class CUserServer : public CServer
{
public:
	CUserServer();
	~CUserServer();
public:
	int	 Initialize();
public:
	const char* GetRedisHost() const;
	const unsigned short GetRedisPort() const;
	const char* GetRedisAuth() const;
	bool PostMemDataBaseReq(CServices* pServices,void* pData, DATASIZE uDataSize);
	bool PostMemDataBaseRet(CServices* pServices,SERVICEINDEX nToSerId,SERVICEINDEX nCsid, uint32 nTypeId, void* pData, DATASIZE nDataSize);
private:
	int  ReadConfig(const char* szConfigFile);
private:
	//本进程需要监听的ip、端口
	std::string m_szIp;
	unsigned short m_nPort;
	//中心服地址
	std::string m_szCenterIp;
	unsigned short m_nCenterPort;
	//redis 地址
	std::string m_szRedisHost;
	unsigned short m_nRedisPort;
	std::string m_szAuth;

	CMemDataBaseEnginer* m_pMem;
};