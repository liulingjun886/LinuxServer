#include "MemSink.h"
#include <stdlib.h>
#include <assert.h>
#include <hiredis/hiredis.h>
#include "MemDataDef.h"
#include "MyRedis.h"
#include "MemDataBaseEnger.h"

using namespace std;
using namespace Mem;

CMemSink::CMemSink(CServices* pMemData) :m_pService(pMemData),m_pMemData(NULL)
{
	m_pMemData  = new CMyRedis;
}

CMemSink::~CMemSink()
{
	SAFE_DELTE(m_pMemData);
}

bool CMemSink::Init()
{
	return m_pMemData->Init();
}

bool CMemSink::HandMemDataReq(SERVICEINDEX uFromSerId,SERVICEINDEX nCsid,UINT uTypeId,void *pData, DATASIZE nDataSize)
{
	switch (uTypeId)
	{
		case Mem::USER_LOGIN_REQ:
		{
			UserLoginMemRet ret;
			m_pMemData->Exec(Mem::USER_LOGIN_REQ,pData,nDataSize,&ret,sizeof(UserLoginMemRet));
			CMemDataBaseEnginer::PostMemDataBaseRet(m_pService,uFromSerId,nCsid,Mem::USER_LOGIN_RET,&ret,sizeof(ret));
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
			CMemDataBaseEnginer::PostMemDataBaseRet(m_pService,uFromSerId,nCsid,Mem::USER_JOIN_GAME_RET,&ret,sizeof(ret));
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
	return true;
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