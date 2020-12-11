#pragma once

#include "../Defines.h"
#include "../NetHandSink.h"
#include "../include/TimerNode.h"
#include "../PacketParse.h"

class CGameSerSink : public CNetHandSink
{
public:
	CGameSerSink(CServices* pServices);
	~CGameSerSink();
public:
	virtual bool HandTimeMsg(uint16 uTimeID);
	virtual bool HandNetData(uint16, uint16, CInputPacket& inPacket);
	virtual void Connect();
	virtual void Close();
private:
	bool HandTestNetConn();
	bool HandMsgFromConnSrv(uint16, CInputPacket& inPacket);
private:
	void ConnectSucess(CInputPacket& inPacket);
	void HandMsgFromUserToGame(CInputPacket& inPacket);
	void HandMsgFromUserForRoom(CInputPacket& inPacket);
	void HandMsgFromUserForHall(CInputPacket& inPacket);
private:
	uint16 	m_nConnNo;
	uint8 	m_nTestNum;
	CTimer	m_timer_Link;
};