#pragma once
#include "types.h"

class CServices;

class CNetSink
{
public:
	CNetSink();
	virtual ~CNetSink();
	//static CNetSink* CreateNetSink(CServices* pServices,int nType);
	//static void DestroyNetSink(CNetSink* pSink);
public:
	virtual void Init(const char* szIp) = 0;
	virtual void Connect() = 0;
	virtual bool DisConnect() = 0;
	virtual int  HandNetMsg(SERVICEINDEX nIndex, const char* pData, unsigned uDataSize) = 0;
	virtual bool HandTimeMsg(TIMEERID uTimeID) = 0;
	virtual bool HandUserMsg(int nType, void* pData, DATASIZE uDataSize) = 0;
	virtual void Close() = 0;
	virtual void Destroy() = 0;
};

