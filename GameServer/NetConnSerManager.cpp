#include "NetConnSerManager.h"

CConnSerManager::CConnSerManager():m_pConns(NULL)
{
	m_pConns = new CConnSerInfo[MAX_SER_NUM];
}

CConnSerManager::~CConnSerManager()
{
	if(m_pConns)
	{
		delete[] m_pConns;
		m_pConns = NULL;
	}
}

void CConnSerManager::AddRemoteSer(USHORT nSerNo,USHORT nIndex)
{
	CToolLock lock(&m_rw_lock);
	m_pConns[nSerNo].AddConnSer(nIndex);
}

void CConnSerManager::DelRemoteSer(USHORT nSerNo,USHORT nIndex)
{
	CToolLock lock(&m_rw_lock);
	m_pConns[nSerNo].DelConnSer(nIndex);
}

USHORT CConnSerManager::GetConnSerById(USHORT nSerNo,USHORT nIndex)
{
	CToolLock lock(&m_rw_lock,1);
	return m_pConns[nSerNo].GetConnSerIndex(nIndex);
}

