#pragma once

#include "../Server.h"
#include <string>

class CMemDataBaseEnginer;
class CDataBaseEnginer;
class CServices;

class CDataServer : public CServer
{
public:
	CDataServer();
	~CDataServer();
public:
	int	 Initialize();
public:
	const char* GetRedisHost() const;
	const unsigned short GetRedisPort() const;
	const char* GetRedisAuth() const;

	const char* GetDbHost() const;
	const unsigned short GetDbPort() const;
	const char* GetDbName() const;
	const char* GetDbUserName() const;
	const char* GetDbPass() const;

	SERVICEINDEX GetCenterIndex() const;

	bool PostMemDataBaseReq(CServices* pServices,void* pData, DATASIZE uDataSize);
	bool PostMemDataBaseRet(CServices* pServices,SERVICEINDEX nToSerId,SERVICEINDEX nCsid, uint32 nTypeId, void* pData, DATASIZE nDataSize);

	bool PostDataBaseReq(CServices* pServices,void* pData, DATASIZE uDataSize);
	bool PostDataBaseRet(CServices* pServices,SERVICEINDEX nToSerId,SERVICEINDEX nCsid, uint32 uTypeId, void* pData, DATASIZE uDataSize);
private:
	int  ReadConfig(const char* szConfigFile);
	
private:
	std::string m_szIp;
	unsigned short m_nPort;

	std::string m_szCenterIp;
	unsigned short m_nCenterPort;
	SERVICEINDEX m_nCenterIndex;

	std::string m_szRedisHost;
	unsigned short m_nRedisPort;
	std::string m_szAuth;

	std::string m_szDbHost;
	uint16 		m_nDbPort;
	std::string m_szDbName;
	std::string m_szDbUserName;
	std::string m_szDbPass;

	CMemDataBaseEnginer* m_pMem;
	CDataBaseEnginer*    m_pData;
};