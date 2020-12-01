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
	virtual bool HandNetData(uint16, uint16, uint16, void*, DATASIZE);
	virtual void Connect();
	virtual void Close();
private:
	bool HandMainMsgFromUserSrv(uint16, uint16, void* , DATASIZE);
	bool HandMainMsgFromDataSrv(uint16, uint16, void* , DATASIZE);
	bool HandMainMsgFromGameSrv(uint16, uint16, void* , DATASIZE);
	bool HandMainMsgFromConnSrv(uint16, uint16, void* , DATASIZE);

	bool HandTestNetConn();
	
	void BroadCastGameSerInfo(const GameInfo& gameInfo);
	void SendAllGameSerInfo();
private:
	CTimer m_timer_Link;
	uint16 m_nTestNum;
	
};