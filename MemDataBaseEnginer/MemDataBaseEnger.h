#pragma once
#include "../Defines.h"
#include <assert.h>


class CServices;
class CMemDataBaseEnginer
{
	Single(CMemDataBaseEnginer)
public:
	void SetServiceNum(int nNum);
	inline SERVICEINDEX GetIndex(SERVICEINDEX nRand);
	
	static void PostMemDataBaseReq(CServices* pServices, void* pData, DATASIZE uDataSize);
	static void PostMemDataBaseRet(CServices* pServices,SERVICEINDEX nToSerId,SERVICEINDEX nCsid, uint32 uTypeId, void* pData, DATASIZE uDataSize);
private:
	int m_nServiceNum;
	SERVICEINDEX* m_pIndex;
};
