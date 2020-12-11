#pragma once

#include "../Defines.h"
#include <string>

enum USER_STATE
{
	USER_STATE_NONE=0,
	USER_STATE_LOGINING,
	USER_STATE_LOGINED,
	USER_STATE_CREATING,
	USER_STATE_JOINING,
	
	USER_STATE_WATCH,
	USER_STATE_SIT
};

class CGameUser
{
public:
	CGameUser(UID nUid);
	~CGameUser();
public:
	UID GetUid() const;
	void UserLoginOnLine(uint16 nCid, uint16 nCsid);
	void UserOffLine(uint16 nCid, uint16 nCsid);
	uint32 GetOffLineTime() const;
private:
	UID m_nUserId;
	uint16 m_nCid;
	uint16 m_nCsid;
	uint16 m_nRoomIndex;
	uint16 m_nUserState;
	uint16 m_nGameState;
	std::string m_szUserInfo;
	uint32 m_nOffLineTime;
};