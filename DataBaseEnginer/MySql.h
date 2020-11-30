#pragma once

#include "../include/mysql/mysql.h"
#include "../Defines.h"
#include <string.h>
#include <string>
#include <vector>
#include <map>


class CServices;

struct DBConfig
{
	std::string szHost;
	unsigned short nPort;
	std::string szDbName;
	std::string szDbUser;
	std::string szDbPass;

	DBConfig()
	{
		szHost = "";
		nPort = 0;
		szDbName = "";
		szDbUser = "";
		szDbPass = "";
	}
};

using namespace std;

class CMySql
{
public:
	CMySql(CServices* pService);
	virtual ~CMySql();
public:
	bool InitConnection();
	virtual int  Exec(SERVICEINDEX nIndex, SERVICEINDEX nCsid ,uint32 nType,void* pData,uint32 nDataSize)=0;
protected:
	virtual int  GetDBConfig()=0;
	virtual bool Connected()=0;
protected:
	bool OpenConnect();
	void CloseConnect();

	void SetSpName(const   char* szSpName);
	bool ExecPro();
	bool ExecSql(const char* szSql);
	
	bool HaveNext();
	void MoveNext();
	
	long long GetNumValue(const char* szFild,long long nDef = 0);
	const char* GetStrValue(const char* szFild,const char* szDef = NULL);
	long long GetOutNumValue(const char* szStrFild,long long nDef = 0);
	const char* GetOutStrValue(const char* szFild,const char* szDef = NULL);

	void AddNumParam(long long llValue);
	void AddStrParam(const char* szValue);
	void AddOutParam(const char* szOutParam);
	void Clean();

	void AddStr(const string& szParam);
	bool ExecQuery(const string& szSql1);
	bool ExecSingleQuery(string& szSql);
	bool GetSetResult();
	bool GetSingleResult();
	const char* GetValue(const char* szFild);
	const char* GetOutValue(const char* szFild);
protected:
	DBConfig m_dbConfig;
	CServices* const m_pService;
	MYSQL *m_pMySql;

	string m_szSql;
	string m_szOutSql;
	int m_nInParam;
	int m_nOutParam;

	map<string,int> m_mapFildname;
	vector<vector<string> > m_vecRes;
	map<string,string> m_mapRes;
	vector<vector<string> >::iterator m_it;
};

