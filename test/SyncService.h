#pragma once

#include "../include/Services.h"
#include "../include/TimerNode.h"
#include "../Defines.h"
#include <set>

class CSyncService : public CServices
{
public:
	CSyncService();
	~CSyncService();
protected:
	virtual bool HandData(int nType, SERVICEINDEX nSrcIndex, void *pData, DATASIZE nSize);
	virtual void Initialized();
private:
	bool HandTimeMsg(TIMEERID nTimeId);
	void SyncUserInfo();
private:
	std::set<UID> m_set_sync_user;
	CTimer m_timer_sync;
};