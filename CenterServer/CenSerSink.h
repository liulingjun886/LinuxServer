#pragma once

#include "../NetHandSink.h"
#include <string>
#include <map>
#include "../Defines.h"
#include "../include/ToolLock.h"
#include "../include/TimerNode.h"
#include "../commproto.h"
#include "CenterServer.h"

class CCenSerSink : public CNetHandSink
{
public:
	CCenSerSink(CServices* pServices);
	~CCenSerSink();
public:
	virtual bool HandTimeMsg(uint16 uTimeID);
	virtual bool HandNetData(uint16, uint16, uint16, void*, uint32);
	virtual void Connect();
	virtual void Close();
private:
	bool HandMainMsgFromUserSrv(uint16, uint16, void* , uint32);
	bool HandMainMsgFromDataSrv(uint16, uint16, void* , uint32);
	bool HandMainMsgFromGameSrv(uint16, uint16, void* , uint32);
	bool HandMainMsgFromConnSrv(uint16, uint16, void* , uint32);

	bool HandTestNetConn();
	
	void BroadCastGameSerInfo(const GameInfo& gameInfo);
	void SendAllGameSerInfo();
private:
	CTimer m_timer_Link;
	uint16 m_nTestNum;
	
};