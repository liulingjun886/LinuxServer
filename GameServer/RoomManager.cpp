#include "RoomManager.h"
#include "../Defines.h"
#include <assert.h>
#include <time.h>
//#include "Services.h"
#include "Room.h"
#include "../commproto.h"
#include <stdio.h>
#include "../include/core/Core.h"

#include "GameServer.h"

extern CGameServer* g_pGameServer;

//Single_Init(CRoomManager)
CRoomManager::CRoomManager()
{
}


CRoomManager::~CRoomManager()
{
}

int CRoomManager::AddRoom(uint16 nIndex)
{
	static int sCurRoomid = 0;
	int i = sCurRoomid;
	int nRoomPreId = g_pGameServer->GetSerNo() * 100000;
	int nRoomId = 0;
	do 
	{
		++sCurRoomid;
		if(sCurRoomid >= 100000)
			sCurRoomid = 1;
		
		if(i == sCurRoomid)
			return 0;

		nRoomId = nRoomPreId+sCurRoomid;
		if (m_room_manager.find(nRoomId) == m_room_manager.end())
		{
			m_room_manager[nRoomId] = nIndex;
			break;
		}
	} while (1);
	return nRoomId;
}

void CRoomManager::DestroyRoom(int nRoomId)
{
	if(nRoomId < 100000)
		return;
	map<int, uint16>::iterator it;

	it = m_room_manager.find(nRoomId);
	if (it != m_room_manager.end())
	{
		m_room_manager.erase(it);
	}
}

uint16 CRoomManager::GetRoom(int nRoomId)
{
	map<int, uint16>::iterator it;

	it = m_room_manager.find(nRoomId);
	if (it != m_room_manager.end())
	{
		return it->second;
	}
	return 0;
}

bool CRoomManager::HandData(int nType, SERVICEINDEX nSrcIndex, void *pData, DATASIZE nSize)
{
	
}