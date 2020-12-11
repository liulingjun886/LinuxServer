#pragma once
#include "../include/Sevices.h"
#include "../include/TimerNode.h"
#include "../Defines.h"
#include "PacketParse.h"
#include <map>

class CGameUser;

class CGameUserManager : public CServices
{
public:
	CGameUserManager();
	~CGameUserManager();
protected:
	bool HandData(int nType, SERVICEINDEX nSrcIndex, void *pData, DATASIZE nSize);
	void Initialized();
private:
	void HandUserNetMsg(CInputPacket& inPacket);
	void HandTimeMsg(TIMEERID nTimeID);
	void HandUserMsgFromConnSrv(SERVICEINDEX nSrcIndex,uint16 nSub,CInputPacket& inPacket);
	void ClearOffLineUser();
	
	void JoinRoom();
	void LeaveRoom();
private:
	CGameUser* GetUser(UID nUid);
private:
	typedef std::map<UID,CGameUser*> USERMAP;
	USERMAP m_mapUsers;
	CTimerNode m_time_clear;
};