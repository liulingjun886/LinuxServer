#pragma once
#include <vector>
#include "ToolLock.h"
#include "types.h"

using namespace std;

class CDataSerManager
{
	CRWLock m_rw_lock;
	vector<SERVICEINDEX> m_vecDataSer;
public:
	CDataSerManager();
	~CDataSerManager();
public:
	void AddDataSer(SERVICEINDEX nIndex);
	void DelDataSer(SERVICEINDEX nIndex);
	SERVICEINDEX GetDataSer(int nRand);
};