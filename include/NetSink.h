#pragma once
#include "types.h"

class CServices;
class CNetHandSink;

class CNetSink
{
	static USHORT nHeadSize;
	static USHORT nCommandSize;
	static USHORT nMinDataSize;

	CNetHandSink* m_pSink;
public:
	CNetSink(CServices* pServices,int nType = 1);
	 ~CNetSink();
	static void SendData(CServices* pService,SERVICEINDEX nIndex, USHORT nMain,USHORT nSub,void* pData = NULL,DATASIZE nDataSize = 0);
public:
	int HandNetMsg(SERVICEINDEX nIndex, void* pData, DATASIZE uDataSize);
	bool HandTimeMsg(TIMEERID uTimeID);
	bool DisConnect();
	void Init(UINT nIp);
	void Init(const char* szIp);
	void Close();
	void Connect();
	void Accepted();
	bool HandUserMsg(int nType, void* pData, DATASIZE uDataSize);
};

