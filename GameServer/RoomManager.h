#pragma once
#include <map>
#include <vector>
#include "../include/ToolLock.h"
#include <assert.h>
#include "../Defines.h"
#include "../include/Services.h"
using namespace std;

class CRoomManager : public CServices
{
	Single(CRoomManager)
protected:
	bool HandData(int nType, SERVICEINDEX nSrcIndex, void *pData, DATASIZE nSize);
public:
	int AddRoom(uint16 nIndex);
	void DestroyRoom(int nRoomId);
	uint16 GetRoom(int);
private:
	map<int, uint16> m_room_manager;
};