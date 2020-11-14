#pragma once

#include "NetSink.h"

class CServices;
class CNetHandSink;

class CNetSinkObj : public CNetSink
{
	CNetHandSink* m_pSink;
public:
	CNetSinkObj(CServices* pServices,int nConnType);
	~CNetSinkObj();
	static void SendData(CServices* pService,SERVICEINDEX nIndex, USHORT nMain,USHORT nSub,void* pData = NULL,DATASIZE nDataSize = 0);
public:
	virtual void Init(const char* szIp);
	virtual int HandNetMsg(SERVICEINDEX nIndex, void* pData, DATASIZE uDataSize);
	virtual bool HandTimeMsg(TIMEERID uTimeID);
	virtual bool HandUserMsg(int nType, void* pData, DATASIZE uDataSize);
	virtual bool DisConnect();
	virtual void Close();
	virtual void Connect();
	virtual void Accepted();
};