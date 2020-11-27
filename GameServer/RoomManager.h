#pragma once
#include <map>
#include <vector>
#include "../include/ToolLock.h"
#include <assert.h>
#include "../Defines.h"
using namespace std;

class CRoomManager
{
	Single(CRoomManager)
public:
	int AddRoom(uint16);
	void DestroyRoom(int);
	uint16 GetRoom(int);
private:
	map<int, uint16> m_room_manager;
	CRWLock m_rwlock;
	vector<int> m_vecRoomId;
};