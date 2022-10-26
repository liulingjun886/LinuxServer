#include "MemSink.h"
#include <stdlib.h>
#include <assert.h>
#include "../include/hiredis/hiredis.h"
#include "MyRedis.h"
#include "MemDataBaseEnger.h"
#include "../UserServer/UserRedis.h"
#include "../DataServer/DataRedis.h"
#include "../Server.h"

extern CServer* g_pSer;

CMemSink::CMemSink(CServices* pMemData) : m_pMemData(NULL)
{
	if(SRV_TYPE_USER == g_pSer->GetSerType())
		m_pMemData = new CUserRedis(pMemData);
	else if(SRV_TYPE_DATA == g_pSer->GetSerType())
		m_pMemData = new CDataRedis(pMemData);
}

CMemSink::~CMemSink()
{
	SAFE_DELTE(m_pMemData);
}

bool CMemSink::Init()
{
	return m_pMemData->InitConnection();
}

bool CMemSink::HandMemDataReq(SERVICEINDEX nFromSerId,void *pData, DATASIZE nDataSize)
{
	static DATASIZE nHeadSize = sizeof(DataCenter) + sizeof(uint32);
	if(nDataSize < nHeadSize)
		return false;
		
	DataCenter* pCsid = (DataCenter*)pData;
	uint32* pType = (uint32*)(pCsid+1);
	
	return m_pMemData->Exec(nFromSerId, pCsid->nCsid, *pType, pType+1, nDataSize - nHeadSize);
	/*switch (uTypeId)
	{
		case Mem::USER_LOGIN_REQ:
		{
			UserLoginMemRet ret;
			m_pMemData->Exec(Mem::USER_LOGIN_REQ,pData,nDataSize,&ret,sizeof(UserLoginMemRet));
			//CMemDataBaseEnginer::PostMemDataBaseRet(m_pService,uFromSerId,nCsid,Mem::USER_LOGIN_RET,&ret,sizeof(ret));
			break;
		}
		case Mem::USER_LOGOUT_REQ:
		{
			if(nDataSize != sizeof(Mem::UserLogOutMemReq))
				break;
			m_pMemData->Exec(Mem::USER_LOGOUT_REQ,pData,nDataSize);
			break;
		}
		case Mem::USER_JOIN_GAME_REQ:
		{
			if(nDataSize != sizeof(Mem::UserJoinGameReq))
				break;
			
			Mem::UserJoinGameRet ret;
			m_pMemData->Exec(Mem::USER_JOIN_GAME_REQ, pData, nDataSize, &ret, sizeof(Mem::UserJoinGameRet));
			//CMemDataBaseEnginer::PostMemDataBaseRet(m_pService,uFromSerId,nCsid,Mem::USER_JOIN_GAME_RET,&ret,sizeof(ret));
			break;
		}
		case Mem::USER_QUIT_GAME_REQ:
		{
			if(nDataSize != sizeof(Mem::UserQuitGameReq))
				break;
			m_pMemData->Exec(Mem::USER_QUIT_GAME_REQ, pData, nDataSize);
			break;
		}
		default:
			break;
	}
	return true;*/
}

bool CMemSink::HandTimeMsg(TIMEERID nTimeId)
{
	switch(nTimeId)
	{
		default:
			break;
	}
	return true;
}