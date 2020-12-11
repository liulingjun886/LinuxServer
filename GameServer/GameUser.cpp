#include "GameUser.h"

CGameUser::CGameUser(UID nUid):m_nUserId(nUid)
{
	
}

CGameUser::~CGameUser()
{
	
}

UID CGameUser::GetUid() const
{
	return m_nUserId;
}

uint32 CGameUser::GetOffLineTime() const
{
	return m_nOffLineTime;
}


void CGameUser::UserOffLine(uint16 nCid, uint16 nCsid)
{
	if(nCid == m_nCid && nCsid == m_nCsid)
	{
		m_nCsid = 0;
		m_nLastUpdatTime = (uint32)time(NULL);
		m_nUserState = USER_STATE_NONE;
	}
}

