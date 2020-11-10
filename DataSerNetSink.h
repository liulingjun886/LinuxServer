#pragma once
#include "NetHandSink.h"
#include "MemDataBaseEnger.h"

class CServices;

class CDataSerNetSink : public CNetHandSink
{
	int m_nTestLink;
public:
	CDataSerNetSink(CServices* m_pNetSer);
	~CDataSerNetSink();
public:
	void Connect();
	bool HandNetData(USHORT,USHORT, USHORT, void*, USHORT);
	bool HandTimeMsg(USHORT uTimeId);
private:
	bool TestNetLink();
	bool HandMainMsgCenter(USHORT,USHORT,void*,USHORT);
	bool HandMainMsgNet(USHORT,USHORT, void*, USHORT);
};