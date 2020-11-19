#include "DataSerManager.h"

CDataSerManager::CDataSerManager()
{
	
}

CDataSerManager::~CDataSerManager()
{
	
}

void CDataSerManager::AddDataSer(SERVICEINDEX nIndex)
{
	CToolLock lock(&m_rw_lock);
	m_vecDataSer.push_back(nIndex);
}
void CDataSerManager::DelDataSer(SERVICEINDEX nIndex)
{
	CToolLock lock(&m_rw_lock);
	for(vector<SERVICEINDEX>::iterator it = m_vecDataSer.begin(); it != m_vecDataSer.end(); it++)
	{
		if(*it == nIndex)
		{
			m_vecDataSer.erase(it);
			return;
		}
	}
}
SERVICEINDEX CDataSerManager::GetDataSer(int nRand)
{
	CToolLock lock(&m_rw_lock,1);
	size_t nSize = m_vecDataSer.size();
	if(nSize == 0)
		return INVALID_SERIVCE_INDEX;

	return m_vecDataSer[nRand%nSize];
}


