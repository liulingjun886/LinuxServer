#include "MySql.h"
#include "../include/Services.h"
#include <stdio.h>
#include <stdlib.h>

CMySql::CMySql(CServices* pService):m_pService(pService),m_pMySql(NULL)
{
	Clean();
}

CMySql::~CMySql()
{
	Clean();
	CloseConnect();
	mysql_library_end();
}

bool CMySql::InitConnection()
{
	if(0 != GetDBConfig())
		return false;

	if(!OpenConnect())
		return false;

	if(!Connected())
		return false;

	return true;
}


bool CMySql::OpenConnect()
{
	m_pMySql = mysql_init(NULL);
	if(!m_pMySql)
		return false;
	m_pMySql = mysql_real_connect(m_pMySql, m_dbConfig.szHost.c_str(), m_dbConfig.szDbUser.c_str(), m_dbConfig.szDbPass.c_str(), m_dbConfig.szDbName.c_str(),	m_dbConfig.nPort, 0, CLIENT_MULTI_STATEMENTS);

	if(!m_pMySql)
	{
		printf("m_pMySql connect failer\n");
		return false;
	}
	return true;
}

void CMySql::CloseConnect()
{
	if(m_pMySql)
	{
		mysql_close(m_pMySql);
		m_pMySql = NULL;
	}
}

bool CMySql::ExecPro()
{	
	m_szSql += ");";
	if(m_nOutParam > 0)
	{
		m_szOutSql += ";";
		m_szSql +=  m_szOutSql;
	}

	if(!ExecQuery(m_szSql))
		return false;
	m_it = m_vecRes.begin();
	return true;
}

bool CMySql::HaveNext()
{
	return m_it != m_vecRes.end();
}

void CMySql::MoveNext()
{
	m_it++;
}

long long CMySql::GetNumValue(const char* szFild,long long nDef)
{
	long long llValue;
	const char* szValue = GetValue(szFild);
	try
	{
		llValue = atoll(szValue);
	}
	catch(...)
	{
		llValue = nDef;
	}
	return llValue;
}

const char* CMySql::GetStrValue(const char* szFild,const char* szDef)
{
	const char* szValue = GetValue(szFild);
	if(szValue)
		return szValue;
	return szDef;
}
	
long long CMySql::GetOutNumValue(const char* szFild,long long nDef)
{
	long long llValue;
	const char* szValue = GetOutValue(szFild);
	try
	{
		llValue = atoll(szValue);
	}
	catch(...)
	{
		llValue = nDef;
	}
	return llValue;
}

const char* CMySql::GetOutStrValue(const char* szFild,const char* szDef)
{
	const char* szValude = GetOutValue(szFild);
	if(szValude)
		return szValude;
	return szDef;
}

const char* CMySql::GetValue(const char* szFild)
{
	if(m_it == m_vecRes.end())
		return NULL;

	map<string,int>::iterator it = m_mapFildname.find(szFild);
	if(it == m_mapFildname.end())
		return NULL;

	return (*m_it)[it->second].c_str();
}

const char* CMySql::GetOutValue(const char* szFild)
{
	string str(szFild);
	map<string,string>::iterator it = m_mapRes.find(str);
	if(it != m_mapRes.end())
		return it->second.c_str();
	return NULL;
}

void CMySql::SetSpName(const    char* szSpName)
{
	Clean();
	m_szSql = "call " + string(szSpName);
}

void CMySql::AddNumParam(long long llValue)
{
	char szData[32] = {0};
	sprintf(szData,"%lld",llValue);
	AddStr(szData);
}

void CMySql::AddStrParam(const char* szValue)
{
	string szData(szValue);
	
	string::size_type  pos(0); 
	for( pos = szData.find("'",pos); pos != string::npos; pos = szData.find("'",++pos))
  		szData.replace(pos++,1,"\\\'");

	szData = "\'" + szData + "\'";
	AddStr(szData);
}

void CMySql::AddOutParam(const char* szParam)
{
	string str(szParam);
	AddStr(str);
	if(m_nOutParam == 0)
	{
		m_szOutSql += "select " + str;
	}
	else
	{
		m_szOutSql += "," + str;
	}
	++m_nOutParam;
}

bool CMySql::ExecSql(const char* szSql)
{
	Clean();
	return ExecQuery(szSql);
}

void CMySql::Clean()
{
	m_mapFildname.clear();
	m_vecRes.clear();
	m_mapRes.clear();
	
	m_szSql.clear();
	m_szOutSql.clear();
	m_nInParam = 0;
	m_nOutParam = 0;
}

void CMySql::AddStr(const string& szParam)
{
	if(m_nInParam == 0)
	{
		m_szSql += "(" + szParam;
	}
	else
	{
		m_szSql += "," + szParam;
	}
	++m_nInParam;
}

bool CMySql::ExecQuery(const string& szSql)
{
	printf("szSql = %s\n",szSql.c_str());
redo:	int ret = mysql_real_query(m_pMySql, szSql.c_str(), szSql.length());
	if(ret != 0)
	{
		printf("mysql_real_query failer = %s,szSql = %s\n",mysql_error(m_pMySql),szSql.c_str());
		CloseConnect();
		if(OpenConnect())
			goto redo;
		return false;
	}
	
#if 0
	int nResultNum = 0;
	do
	{
		if(nResultNum == 0)
		{
			GetSetResult();
		}
		else
		{
			GetSingleResult(nResultNum);
		}
		++nResultNum;
		ret = mysql_next_result(m_pMySql);
		printf("Ret = %d\n",ret);
	}
	while (!ret); //这里必须mysql_next_result 返回非0*/
#else
	GetSetResult();
	while(!mysql_next_result(m_pMySql))
	{
		GetSingleResult();
	}
#endif	
	return true;
}

bool CMySql::ExecSingleQuery(string& szSql)
{
	int ret = mysql_real_query(m_pMySql, szSql.c_str(), szSql.length());
	if(ret != 0)
	{
		printf("mysql_real_query failer = %s\n",mysql_error(m_pMySql));
		return false;
	}
	do
	{
		MYSQL_RES* Res = mysql_store_result(m_pMySql);
		if(!Res)
			break;
		MYSQL_FIELD *pFilds = NULL;
		MYSQL_ROW Row = mysql_fetch_row(Res);
		int nIndex = 0;
		while((pFilds = mysql_fetch_field(Res)))
		{
			m_mapRes[pFilds->name] = Row[nIndex++];
		}
		mysql_free_result(Res);
	}while (!mysql_next_result(m_pMySql));
	return true;
}

bool CMySql::GetSetResult()
{
	MYSQL_RES* Res = mysql_store_result(m_pMySql);
	printf("affected_rows1 = %lld\n",m_pMySql->affected_rows);
	if(!Res)
		return true;
	printf("GetSetResult\n");
	MYSQL_FIELD *pFilds = NULL;
	int nFildNum = 0;
	while((pFilds = mysql_fetch_field(Res)))
	{
		m_mapFildname[pFilds->name] = nFildNum++;
	}
	MYSQL_ROW row = NULL;
	while((row = mysql_fetch_row(Res)))
	{
		vector<string> vecStr;
		for(int i = 0; i < nFildNum; i++)
		{
			vecStr.push_back(string(row[i]));
		}
		m_vecRes.push_back(vecStr);
	}
	mysql_free_result(Res);
	return true;
}
bool CMySql::GetSingleResult()
{
	MYSQL_RES* Res = mysql_store_result(m_pMySql);
	printf("affected_rows2 = %lld\n",m_pMySql->affected_rows);
	if(!Res)
		return true;
	
	MYSQL_FIELD *pFilds = NULL;
	MYSQL_ROW Row = mysql_fetch_row(Res);
	int nIndex = 0;
	while((pFilds = mysql_fetch_field(Res)))
	{
		m_mapRes[pFilds->name] = Row[nIndex++];
	}
	mysql_free_result(Res);
	return true;
}

