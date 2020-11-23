#pragma once

#include "../include/types.h"

class CGameSerSink : public CNetHandSink
{
public:
	CGameSerSink(CServices* pServices);
	~CGameSerSink();
public:
	virtual bool HandTimeMsg(USHORT uTimeID);
	virtual bool HandNetData(USHORT, USHORT, USHORT, void*, UINT);
	virtual void Connect();
	virtual bool DisConnect();
private:
	bool HandTestNetConn();
	bool HandMsgFromConnSrv(USHORT, USHORT, void*, UINT);
private:
	USHORT 	m_nConnNo;
	BYTE 	m_nTestNum;
};