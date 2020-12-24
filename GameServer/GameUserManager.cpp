#include "GameUserManager.h"
#include "GameUser.h"

enum TIME_ID
{
	TIME_ID_CLEAR_OFFLINE_USER=1,
};

CGameUserManager::CGameUserManager()
{
	m_time_clear.InitTimerObj(this, TIME_ID_CLEAR_OFFLINE_USER);
	m_mapUsers.clear();
}

CGameUserManager::~CGameUserManager()
{
	std::map<UID,CGameUser*>::iterator it = m_mapUsers.begin();
	for(; it != m_mapUsers.end(); it++)
	{
		SAFE_DELTE(it->second);
	}
	m_mapUsers.clear();
}

void CGameUserManager::Activated()
{
	m_time_clear.StartTimerSec(3600);
}


bool CGameUserManager::HandData(int nType, SERVICEINDEX nSrcIndex, void *pData, DATASIZE nSize)
{
	switch(nType)
	{
		case USER_NET_MSG:
		{
			CInputPacket inPacket;
			inPacket.Copy((char*)pData, nSize);
			HandUserNetMsg(nSrcIndex,inPacket);
			break;
		}
		case TIME_MSG:
		{
			TIMEERID nTimeId = *(TIMEERID*)pData;
			HandTimeMsg(nTimeId);
			break;
		}
	}
	return true;
}

CGameUser* CGameUserManager::GetUser(UID nUid)
{
	CGameUser* pUser = NULL;
	USERMAP::iterator it = m_mapUsers.find(nUid);
	if(it != m_mapUsers.end())
	{
		pUser = it->second;
	}
	
	if(NULL == pUser)
	{
		pUser = new CGameUser(nUid);
		if(NULL != pUser)
		{
			m_mapUsers[nUid] = pUser;
		}	
	}
	return pUser;
}


void CGameUserManager::HandUserNetMsg(SERVICEINDEX nSrcIndex,CInputPacket& inPacket)
{
	uint16 nMain = inPacket.GetMainCmd();
	uint16 nSub = inPacket.GetSubCmd();
	
	switch(nMain)
	{
		case MAIN_MSG_CONNSER:
		{
			HandUserMsgFromConnSrv(nSrcIndex,nSub,inPacket);
			break;
		}
		case MAIN_MSG_DATASER:
		{
			break;
		}
		default:
		{
			break;
		}
	}
	return;
}

void CGameUserManager::ClearOffLineUser()
{
	USERMAP::iterator it = m_mapUsers.begin();
	uint32 nNow = (uint32)time(NULL);
	while(it != m_mapUsers.end())
	{
		CGameUser* pUser = it->second;
		if(NULL == pUser)
		{
			m_mapUsers.erase(it++);
			continue;
		}

		uint32 nOffLimeTime = pUser->GetOffLineTime();
		if(0 == nOffLimeTime)
		{
			++it;
			continue;
		}

		if(nNow - nOffLimeTime > 18000)
		{
			m_mapUsers.erase(it++);
			continue;
		}

		++it;
	}

	m_time_clear.StartTimerSec(3600);
}


void CGameUserManager::HandTimeMsg(TIMEERID nTimeID)
{
	switch(nTimeID)
	{
		case TIME_ID_CLEAR_OFFLINE_USER:
		{
			ClearOffLineUser();
			break;
		}
		default:
		{
			break;
		}
	}
}


void CGameUserManager::HandUserMsgFromConnSrv(SERVICEINDEX nSrcIndex, uint16 nSub, CInputPacket& inPacket)
{
	switch(nSub)
	{
		case CS_SUB_MSG_USER_LOGIN_HALL:
			break;
		default:
			break;
	}
	
	UID nUid = inPacket.ReadInt32();
	
	//CGameUser*	
}

