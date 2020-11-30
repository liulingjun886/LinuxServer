#pragma once
#include "../Defines.h"
#include <assert.h>

class CServices;

class CDataBaseEnginer
{
public:
	CDataBaseEnginer();
	~CDataBaseEnginer();
public:
	void SetServiceNum(int nNum);
	SERVICEINDEX GetIndex(int nRand);
private:
	int m_nServiceNum;
	SERVICEINDEX* m_pIndex;
};

