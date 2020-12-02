#pragma once
#include "../Defines.h"
#include <assert.h>


class CServices;
class CMemDataBaseEnginer
{
public:
	CMemDataBaseEnginer();
	~CMemDataBaseEnginer();
public:
	void SetServiceNum(int nNum);
	SERVICEINDEX GetIndex(SERVICEINDEX nRand);
private:
	int m_nServiceNum;
	SERVICEINDEX* m_pIndex;
};
