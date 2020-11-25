#pragma once

#include "../include/types.h"
#include "../NetHandSink.h"

class ConnSucess;

class CGameSerSink : public CNetHandSink
{
public:
	CGameSerSink(CServices* pServices);
	~CGameSerSink();
public:
	virtual bool HandTimeMsg(USHORT uTimeID);
	virtual bool HandNetData(USHORT, USHORT, USHORT, void*, UINT);
	virtual void Connect();
	virtual void Close();
private:
	bool HandTestNetConn();
	bool HandMsgFromConnSrv(USHORT, USHORT, void*, UINT);
private:
	void ConnectSucess(ConnSucess* pConn);
private:
	USHORT 	m_nConnNo;
	BYTE 	m_nTestNum;
};