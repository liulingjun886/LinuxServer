#include "DataBaseSink.h"
#include "MySql.h"
#include <stdio.h>
#include "../commproto.h"
#include "Services.h"
#include "DataBaseDef.h"
#include "DataBaseEnginer.h"

using namespace DataBase;

CDataBaseSink::CDataBaseSink(CServices    * pService) : m_pService(pService)
{
	m_pDataBase = new CMySql;
}

bool CDataBaseSink::Init()
{
	if(!m_pDataBase->Init())
	{
		printf("m_pDataBase->Init() false\n");
		return false;
	}
	return true;
}

CDataBaseSink::~CDataBaseSink()
{
	SAFE_DELTE(m_pDataBase);
}

bool CDataBaseSink::HandDataBaseReq(SERVICEINDEX nFromIndex,SERVICEINDEX nCsid, UINT nType,void* pData,DATASIZE nDataSize)
{
	switch(nType)
	{
		case DataBase::USER_LOGIN_REQ:
		{
			UserLogin(nFromIndex,nCsid,pData,nDataSize);
			break;
		}
		default:
			break;
	}
	return true;
}

bool CDataBaseSink::HandTimeMsg(TIMEERID nTimeId)
{
	return true;
}


void CDataBaseSink::UserLogin(USHORT nIndex,SERVICEINDEX nCsid,void* pData,USHORT nDataSize)
{
	if(nDataSize != sizeof(UserLoginReq))
		return;
	UserLoginReq* pLogin = (UserLoginReq*)pData;
	m_pDataBase->SetSpName("UserLogin");
	m_pDataBase->AddNumParam(pLogin->nUserId);
	m_pDataBase->AddStrParam(pLogin->szPass);
	m_pDataBase->AddOutParam("@ret");
	m_pDataBase->ExecPro();
	
	int ret = (int)m_pDataBase->GetOutNumValue("@ret");
	UserLoginRet LoginRet;
	LoginRet.ret = ret;
	LoginRet.nUserId = pLogin->nUserId;
	LoginRet.nSex = 1;
	strcpy(LoginRet.szName,"LiLei");
	strcpy(LoginRet.szHeadUrl,"szHeadUrl");
	CDataBaseEnginer::PostDataBaseRet(m_pService, nIndex, nCsid, DataBase::USER_LOGIN_RET,&LoginRet,sizeof(UserLoginRet));
}

