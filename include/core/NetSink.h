#pragma once
#include "types.h"

class CServices;

class CNetSink
{
public:
	CNetSink();
	virtual void Destroy();
protected:
	virtual ~CNetSink();
public:
	virtual void Init(const char* szIp) = 0;
	virtual void Connect() = 0;
	virtual bool DisConnect() = 0;
	virtual int  HandNetMsg(SERVICEINDEX nIndex, const char* pData, unsigned uDataSize) = 0;
	virtual bool HandTimeMsg(TIMEERID uTimeID) = 0;
	virtual bool HandUserMsg(int nType, void* pData, DATASIZE uDataSize) = 0;
	virtual void Close() = 0;
};

