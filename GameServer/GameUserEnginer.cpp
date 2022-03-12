#include "GameUserEnginer.h"
#include "GameUserManager.h"
#include "../include/core/Core.h"
#include "../include/core/SingleObject.h"


//Single_Init(CGameUserEnginer)

CGameUserEnginer::CGameUserEnginer()
{
	memset(m_arrIndex,0,sizeof(m_arrIndex));
	m_mapManagerInfo.clear();
}

CGameUserEnginer::~CGameUserEnginer()
{
	m_mapManagerInfo.clear();
}

bool CGameUserEnginer::Init()
{
	for(int i = 0; i < MAX_GAMEUSER_MANAGER; ++i)
	{
		CGameUserManager* pGameUser = new CGameUserManager;
		if(!CSingleObject<CCore>::Instance()->AddService(pGameUser))
			return false;
		m_arrIndex[i] = pGameUser->GetServiceIndex();
	}
	return true;
}

SERVICEINDEX CGameUserEnginer::GetUserManagerIndex(UID nUserId)
{
	return m_arrIndex[nUserId%MAX_GAMEUSER_MANAGER];
}

bool CGameUserEnginer::HandData(int nType, SERVICEINDEX nSrcIndex, void *pData, DATASIZE nSize)
{
	return true;
}

