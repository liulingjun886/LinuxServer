#pragma once

#include "../NetHandSink.h"
#include <string>
#include <map>
#include "../Defines.h"
#include "../include/core/ToolLock.h"
#include "../include/core/TimerNode.h"
#include "../commproto.h"
#include "CenterServer.h"
#include "../PacketParse.h"

class CCenSerSink : public CNetHandSink
{
public:
	CCenSerSink(CServices* pServices);
	~CCenSerSink();
public:
	virtual bool HandTimeMsg(uint16 uTimeID);
	virtual bool HandNetData(uint16, uint16, CInputPacket& pInPack);
	virtual void Connect();
	virtual void Close();
private:
	bool HandMainMsgFromUserSrv(uint16, CInputPacket& inPacket);
	bool HandMainMsgFromDataSrv(uint16, CInputPacket& inPacket);
	bool HandMainMsgFromGameSrv(uint16, CInputPacket& inPacket);
	bool HandMainMsgFromConnSrv(uint16, CInputPacket& inPacket);

	bool HandTestNetConn();
	
	void BroadCastGameSerInfo(const GameInfo& gameInfo);
	void SendAllGameSerInfo();
private:
	CTimer m_timer_Link;
	uint16 m_nTestNum;
	
};