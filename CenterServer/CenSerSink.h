#pragma once

#include "../NetHandSink.h"
#include <string>
#include <map>
#include "../include/types.h"
#include "../include/ToolLock.h"
#include "../include/TimeNode.h"
#include "../commproto.h"
#include "CenterServer.h"

class CCenSerSink : public CNetHandSink
{
public:
	CCenSerSink(CServices* pServices);
	~CCenSerSink();
public:
	virtual bool HandTimeMsg(USHORT uTimeID);
	virtual bool HandNetData(USHORT, USHORT, USHORT, void*, UINT);
	virtual void Connect();
	virtual bool DisConnect();
private:
	bool HandMainNetMsg(USHORT, USHORT, void* , UINT);
	void BroadCastGameSerInfo(const GameInfo& gameInfo);
	void SendAllGameSerInfo();
private:
	CTimer m_timerTestLink;
	unsigned m_nTestNum;
	CCenterServer* m_pServer;
};