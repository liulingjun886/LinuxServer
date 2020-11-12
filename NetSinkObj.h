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
	int HandNetMsg(SERVICEINDEX nIndex, void* pData, DATASIZE uDataSize);
	bool HandTimeMsg(TIMEERID uTimeID);
	bool HandUserMsg(int nType, void* pData, DATASIZE uDataSize);
	bool DisConnect();
	void Init(UINT nIp);
	void Init(const char* szIp);
	void Close();
	void Connect();
	void Accepted();
};