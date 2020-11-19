#pragma once
#include <map>
#include <vector>
#include "types.h"
#include "ToolLock.h"
using namespace std;

class CGameSerInfo
{
	vector<USHORT> m_GameSerIndex;
public:
	CGameSerInfo()
	{
		m_GameSerIndex.clear();
	}
	~CGameSerInfo()
	{
		m_GameSerIndex.clear();
	}
public:
	USHORT GetGameSerIndex(USHORT nIndex)
	{
		if(m_GameSerIndex.size() == 0)
		{
			return INVALID_SERIVCE_INDEX;
		}
		nIndex = nIndex%m_GameSerIndex.size();
		return m_GameSerIndex[nIndex];
	}

	bool AddGameSer(USHORT nIndex)
	{
		if(m_GameSerIndex.size() == MAX_LINK_CONN)
			return false;
		
		for(vector<USHORT>::iterator it = m_GameSerIndex.begin(); it != m_GameSerIndex.end(); it++)
		{
			if(*it == nIndex)
			{
				return false;
			}
		}
		m_GameSerIndex.push_back(nIndex);
		return true;
	}

	bool DelGameSer(USHORT nIndex)
	{
		for(vector<USHORT>::iterator it = m_GameSerIndex.begin(); it != m_GameSerIndex.end(); it++)
		{
			if(*it == nIndex)
			{
				m_GameSerIndex.erase(it);
				break;
			}
		}
		return m_GameSerIndex.size() != 0;
	}
};

class CGameSerManager
{
	CRWLock m_rw_lock;
	CGameSerInfo* m_pGameSer;
	map<UINT,vector<USHORT> > m_mapGameSer;
public:
	CGameSerManager();
	~CGameSerManager();
public:
	bool AddRemoteSer(UINT,USHORT,USHORT);
	void DelRemoteSer(UINT,UINT,UINT);
	USHORT RandSer(USHORT);
	USHORT RandSerByGameId(UINT,USHORT);
	USHORT GetSerBySerNo(USHORT,USHORT);

	USHORT GetSerNum();
};
