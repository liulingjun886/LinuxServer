#include "SyncService.h"
#include "DataServer.h"

enum en_time_id
{
	TIME_ID_SYNC = 1,
};

#define SYNC_TIME 10

extern CDataServer* g_pDataServer;

CSyncService::CSyncService()
{
	m_set_sync_user.clear();
	m_timer_sync.InitTimerObj(this, TIME_ID_SYNC);
}

CSyncService::~CSyncService()
{
	m_set_sync_user.clear();
}

void CSyncService::Initialized()
{
	m_timer_sync.StartTimerSec(SYNC_TIME);
}


bool CSyncService::HandData(int nType, SERVICEINDEX nSrcIndex, void *pData, DATASIZE nSize)
{
	switch(nType)
	{
		case TIME_MSG:
		{
			TIMEERID nTimeId = *(TIMEERID*)pData;
			return HandTimeMsg(nTimeId);
		}
	}
}

bool CSyncService::HandTimeMsg(TIMEERID nTimeId)
{
	switch(nTimeId)
	{
		case TIME_ID_SYNC:
		{
			SyncUserInfo();
			return true;
		}
		default:
			break;
	}
	return true;
}

void CSyncService::SyncUserInfo()
{
	if(0 == m_set_sync_user.size())
		return;
	SyncInfoToDataBase sync;
	std::set<UID>::iterator it = m_set_sync_user.begin();
	for(; it != m_set_sync_user.end(); it++)
	{
		//PostData(g_pDataServer->, int nType, void * pData = 0, DATASIZE size = 0)
		sync.nEvType = SYNC_INFO_DATABASE;
		g_pDataServer->PostMemDataBaseReq(this, &sync, sizeof(sync));
	}
	m_set_sync_user.clear();
}

