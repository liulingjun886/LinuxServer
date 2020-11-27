#pragma once
#include "../NetHandSink.h"
#include "../MemDataBaseEnginer/MemDataBaseEnger.h"
#include "DataServer.h"

class CServices;

class CDataSerSink : public CNetHandSink
{
public:
	CDataSerSink(CServices* m_pNetSer);
	~CDataSerSink();
public:
	void Connect();
	bool HandNetData(uint16,uint16, uint16, void*, uint16);
	bool HandTimeMsg(uint16 uTimeId);
private:
	bool TestNetLink();
	bool HandMainMsgFromGameSer(uint16,uint16,void*,uint16);
private:
	int m_nTestLink;
};