#pragma once
#include <map>
#include <vector>
#include "ToolLock.h"
#include <assert.h>
#include "types.h"
using namespace std;

class CRoomManager
{
	Single(CRoomManager)
public:
	int AddRoom(USHORT);
	void DestroyRoom(int);
	USHORT GetRoom(int);
private:
	map<int, USHORT> m_room_manager;
	CRWLock m_rwlock;
	vector<int> m_vecRoomId;
};