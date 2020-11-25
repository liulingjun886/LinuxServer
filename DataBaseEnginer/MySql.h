#pragma once
#include "../include/interface.h"
#include "../include/mysql/mysql.h"
#include <string.h>
#include <vector>
#include <map>
using namespace std;

typedef struct MySql_Config
{
	char szHost[16];
	int nPort;
	char szDbName[16];
	char szUser[16];
	char szPass[16];
	MySql_Config()
	{
		memset(this,0,sizeof(MySql_Config));
	}
}DBConfig;

class CMySql : public IDataBase
{
	DBConfig m_dbConfig;
	MYSQL *m_pMySql;
	string m_szSql;
	string m_szOutSql;
	int m_nInParam;
	int m_nOutParam;

	map<string,int> m_mapFildname;
	vector<vector<string> > m_vecRes;
	map<string,string> m_mapRes;
	vector<vector<string> >::iterator m_it;
public:
	CMySql();
	~CMySql();
public:
	bool Init();
	bool OpenConnection();
	void CloseConnection();

	void SetSpName(const    char* szSpName);
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
private:
	void AddStr(const string& szParam);
	bool ExecQuery(const string& szSql1);
	bool ExecSingleQuery(string& szSql);
	bool GetSetResult();
	bool GetSingleResult();
	bool GetDbCfg();
	const char* GetValue(const char* szFild);
	const char* GetOutValue(const char* szFild);
};
