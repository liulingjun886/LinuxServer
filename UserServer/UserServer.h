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
private:
	int  ReadConfig(const char* szConfigFile);
private:
	std::string m_szIp;
	unsigned short m_nPort;

	std::string m_szCenterIp;
	unsigned short m_nCenterPort;

	std::string m_szRedisHost;
	unsigned short m_nRedisPort;
	std::string m_szAuth;

	CMemDataBaseEnginer* m_pMem;
};