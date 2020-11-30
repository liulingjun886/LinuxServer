#include "DataMySql.h"
#include "DataServer.h"
#include "DataBaseDef.h"

extern CDataServer* g_pDataServer;

using namespace DataBase;

CDataMySql::CDataMySql(CServices* pService):CMySql(pService)
{

}

CDataMySql::~CDataMySql()
{
	
}

int CDataMySql::GetDBConfig()
{
	m_dbConfig.szHost = g_pDataServer->GetDbHost();
	m_dbConfig.szDbName = g_pDataServer->GetDbName();
	m_dbConfig.szDbUser = g_pDataServer->GetDbUserName();
	m_dbConfig.szDbPass = g_pDataServer->GetDbPass();
	m_dbConfig.nPort = g_pDataServer->GetDbPort();
	return 0;

	/*ifstream in("./config/mysql.config");
	if(in.is_open())
	{
		char szConfig[64] = { 0 };
		while (in.getline(szConfig, 64))
		{
			char *szKey = strtok(szConfig, ":");
			char *szValue = strtok(NULL, ":");
			
			if(!szKey)
				continue;
			if(!strcmp(szKey,"Host"))
				strcpy(m_dbConfig.szHost,szValue);
			else if(!strcmp(szKey,"Port"))
				m_dbConfig.nPort = atoi(szValue);
			else if(!strcmp(szKey,"DbName"))
				strcpy((char*)m_dbConfig.szDbName,szValue);
			else if(!strcmp(szKey,"User"))
				strcpy((char*)m_dbConfig.szUser,szValue);
			else if(!strcmp(szKey,"Pass"))
				strcpy((char*)m_dbConfig.szPass,szValue);
		}
		in.close();
	}
	else
	{
		printf("mysql.config not found\n");
		return false;
	}
	return true;*/
}

bool CDataMySql::Connected()
{
	return true;
}


int  CDataMySql::Exec(SERVICEINDEX nIndex, SERVICEINDEX nCsid ,uint32 nType,void* pData,uint32 nDataSize)
{
	switch(nType)
	{
		case DataBase::USER_LOGIN_REQ:
		{
			UserLogin(nIndex,nCsid,pData,nDataSize);
			break;
		}
		default:
			break;
	}
	return true;
}


void CDataMySql::UserLogin(uint16 nIndex,SERVICEINDEX nCsid,void* pData,uint16 nDataSize)
{
	if(nDataSize != sizeof(DataBase::UserLoginReq))
		return;
	
	DataBase::UserLoginReq* pLogin = (DataBase::UserLoginReq*)pData;
	SetSpName("UserLogin");
	AddNumParam(pLogin->nUserId);
	AddStrParam(pLogin->szPass);
	AddOutParam("@ret");
	ExecPro();
	
	int ret = (int)GetOutNumValue("@ret");
	DataBase::UserLoginRet LoginRet;
	LoginRet.ret = ret;
	LoginRet.nUserId = pLogin->nUserId;
	LoginRet.nSex = 1;
	strcpy(LoginRet.szName,"LiLei");
	strcpy(LoginRet.szHeadUrl,"szHeadUrl");
	g_pDataServer->PostDataBaseRet(m_pService, nIndex, nCsid, DataBase::USER_LOGIN_RET, &LoginRet,sizeof(DataBase::UserLoginRet));
}

