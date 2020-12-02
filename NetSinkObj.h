#pragma once

#include "./include/NetSink.h"
#include "PacketParse.h"

class CServices;
class CNetHandSink;

class CNetSinkObj : public CNetSink
{
public:
	CNetSinkObj(CServices* pServices,int nConnType);
	~CNetSinkObj();
	static void SendData(CServices* pService,SERVICEINDEX nIndex, uint16 nMain,uint16 nSub,void* pData = NULL,DATASIZE nDataSize = 0);
	static void SendData(CServices* pService,SERVICEINDEX nIndex, COutputPacket& out);
public:
	virtual void Init(const char* szIp);
	virtual void Connect();
	virtual bool DisConnect();
	virtual int  HandNetMsg(SERVICEINDEX nIndex, const char* pData, unsigned uDataSize);
	virtual bool HandTimeMsg(TIMEERID uTimeID);
	virtual bool HandUserMsg(int nType, void* pData, DATASIZE uDataSize);
	virtual void Close();
private:
	CNetHandSink* m_pSink;
};