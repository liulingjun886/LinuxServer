#pragma once
#include "types.h"
#include <assert.h>

class CServices;

class CDataBaseEnginer
{
	Single(CDataBaseEnginer)
public:
	void SetServiceNum(int nNum);
	inline SERVICEINDEX GetIndex(int nRand);

	static void PostDataBaseReq(CServices* pServices,void* pData, DATASIZE uDataSize);
	static void PostDataBaseRet(CServices* pServices,SERVICEINDEX nToSerId,SERVICEINDEX nCsid, UINT uTypeId, void* pData, DATASIZE uDataSize);
private:
	int m_nServiceNum;
	SERVICEINDEX* m_pIndex;
};

