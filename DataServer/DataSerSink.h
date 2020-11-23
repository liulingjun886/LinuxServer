#pragma once
#include "NetHandSink.h"
#include "MemDataBaseEnger.h"
#include "DataServer.h"

class CServices;

class CDataSerSink : public CNetHandSink
{
public:
	CDataSerSink(CServices* m_pNetSer);
	~CDataSerSink();
public:
	void Connect();
	bool HandNetData(USHORT,USHORT, USHORT, void*, USHORT);
	bool HandTimeMsg(USHORT uTimeId);
private:
	bool TestNetLink();
	bool HandMainMsgFromGameSer(USHORT,USHORT,void*,USHORT);
private:
	int m_nTestLink;
};