#pragma once
#include "../Defines.h"
#include <assert.h>


class CServices;
class CMemDataBaseEnginer
{
	//Single(CMemDataBaseEnginer)

public:
	CMemDataBaseEnginer();
	~CMemDataBaseEnginer();
public:
	void SetServiceNum(int nNum);
	SERVICEINDEX GetIndex(SERVICEINDEX nRand);
	
	//void PostMemDataBaseReq(CServices* pServices, void* pData, DATASIZE uDataSize);
	//void PostMemDataBaseRet(CServices* pServices,SERVICEINDEX nToSerId,SERVICEINDEX nCsid, uint32 uTypeId, void* pData, DATASIZE uDataSize);
private:
	int m_nServiceNum;
	SERVICEINDEX* m_pIndex;
};
