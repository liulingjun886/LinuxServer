#pragma once
#include <map>
#include <vector>
#include "types.h"
#include "ToolLock.h"
using namespace std;

class CConnSerInfo
{
	vector<USHORT> m_vecConns;
public:
	CConnSerInfo()
	{
		m_vecConns.clear();
	}
	~CConnSerInfo()
	{
		m_vecConns.clear();
	}
public:
	void AddConnSer(USHORT nIndex)
	{
		for(vector<USHORT>::iterator it = m_vecConns.begin(); it != m_vecConns.end(); it++)
		{
			if(*it == nIndex)
				return;
		}
		m_vecConns.push_back(nIndex);
	}
	void DelConnSer(USHORT nIndex)
	{
		for(vector<USHORT>::iterator it = m_vecConns.begin(); it != m_vecConns.end(); it++)
		{
			if(*it == nIndex)
			{
				m_vecConns.erase(it);
				return;
			}
		}
	}

	USHORT GetConnSerIndex(USHORT nIndex)
	{
		if(m_vecConns.size() == 0)
			return INVALID_SERIVCE_INDEX;
		nIndex = nIndex%m_vecConns.size();
		return m_vecConns[nIndex];
	}
};

class CConnSerManager
{
	CRWLock m_rw_lock;
	CConnSerInfo* m_pConns;
public:
	CConnSerManager();
	~CConnSerManager();
public:
	void AddRemoteSer(USHORT,USHORT);
	void DelRemoteSer(USHORT,USHORT);

	USHORT GetConnSerById(USHORT,USHORT);
};

