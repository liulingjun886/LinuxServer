#pragma once
#include "types.h"

class CServices;
class CNetHandSink;

enum SOCK_CONN_TYPE
{
	SOCK_CONN_SER,    	//被动连接
	SOCK_CONN_CLI,		//主动连接
};

class CNetSink
{
	CNetHandSink* m_pSink;
public:
	CNetSink(CServices* pServices,int nConnType = SOCK_CONN_SER);
	 ~CNetSink();
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

