#pragma once
#include "../include/Services.h"
#include "../include/TimerNode.h"
#include "../Defines.h"
#include "../PacketParse.h"
#include <assert.h>
#include <map>

#define MAX_GAMEUSER_MANAGER 0xFF

class CGameUserEnginer : public CServices
{
	Single(CGameUserEnginer)
public:
	bool Init();
	SERVICEINDEX GetUserManagerIndex(UID nUserId);
protected:
	bool HandData(int nType, SERVICEINDEX nSrcIndex, void *pData, DATASIZE nSize);
private:
	SERVICEINDEX m_arrIndex[MAX_GAMEUSER_MANAGER];
	std::map<SERVICEINDEX, uint32> m_mapManagerInfo;
};

