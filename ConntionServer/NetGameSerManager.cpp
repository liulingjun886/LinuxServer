#include "NetGameSerManager.h"
#include <assert.h>
#include <stdio.h>


CGameSerManager::CGameSerManager()
{
	m_pGameSer = new CGameSerInfo[MAX_GAME_SER_NUM];
}

CGameSerManager::~CGameSerManager()
{
	
	for(map<UINT,vector<USHORT> >::iterator it = m_mapGameSer.begin();it != m_mapGameSer.end();it++)
	{
		it->second.clear();
	}
	m_mapGameSer.clear();
	
	SAFE_DELTEARRAY(m_pGameSer);
	m_pGameSer = NULL;
}

bool CGameSerManager::AddRemoteSer(UINT nGameId,USHORT nSerNo,USHORT nSerIndex)
{
	CToolLock lock(&m_rw_lock);
	if(nSerNo >= MAX_GAME_SER_NUM)
		return false;
	map<UINT,vector<USHORT> >::iterator it_id = m_mapGameSer.find(nGameId);

	if(it_id != m_mapGameSer.end())
	{
		bool bHave = false;
		vector<USHORT> &nGameNos = m_mapGameSer[nGameId];
		for(vector<USHORT>::iterator it = nGameNos.begin(); it != nGameNos.end(); it++)
		{
			if(*it == nSerNo)
			{
				bHave = true;
				break;
			}
		}
		if(!bHave)
		{
			nGameNos.push_back(nSerNo);
		}
	}
	else
	{
		m_mapGameSer[nGameId].push_back(nSerNo);
	}
	
	return m_pGameSer[nSerNo].AddGameSer(nSerIndex);
}

void CGameSerManager::DelRemoteSer(UINT nGameNo,UINT nGameId,UINT nIndex)
{
	CToolLock lock(&m_rw_lock);
	map<UINT,vector<USHORT> >::iterator it_game = m_mapGameSer.find(nGameId);
	if(it_game != m_mapGameSer.end())
	{
		
		vector<USHORT>& vecGameNos = it_game->second;
		for(vector<USHORT>::iterator it = vecGameNos.begin();it != vecGameNos.end(); it++)
		{
			if(*it == nGameNo)
			{
				vecGameNos.erase(it);
				break;
			}
		}
	}
	m_pGameSer[nGameNo].DelGameSer(nIndex);
}


USHORT CGameSerManager::RandSer(USHORT nIndex)
{
	return m_pGameSer[1].GetGameSerIndex(nIndex);
}


USHORT CGameSerManager::RandSerByGameId(UINT nGameId,USHORT nIndex)
{
	
	map<UINT,vector<USHORT> >::iterator it = m_mapGameSer.end();
	CToolLock lock(&m_rw_lock,1);
	it = m_mapGameSer.find(nGameId);
	if(it == m_mapGameSer.end())
	{
		return 0;
	}

	const vector<USHORT>& vec = it->second;
	if(vec.size() == 0)
	{
		return 0;
	}
	
	int nGameSerNo = vec[rand()%vec.size()];
	return m_pGameSer[nGameSerNo].GetGameSerIndex(nIndex);
}

USHORT CGameSerManager::GetSerBySerNo(USHORT nSerNo,USHORT nIndex)
{
	CToolLock lock(&m_rw_lock,1);
	return m_pGameSer[nSerNo].GetGameSerIndex(nIndex);
}




