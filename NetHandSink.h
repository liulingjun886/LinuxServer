#pragma once
#include "types.h"

class CServices;

class CNetHandSink
{
protected:
	CServices* const m_pNet;
public:
	CNetHandSink(CServices* pServices):m_pNet(pServices){};
	virtual ~CNetHandSink(){};
public:
	virtual bool HandTimeMsg(USHORT uTimeID) {return true;}
	virtual bool HandDataBaseRet(UINT uType,void* pData,USHORT uDataSize){return true;}
	virtual bool HandMemDataRet(UINT uType, void* pData, USHORT uDataSize){return true;}
	virtual void Init(UINT nIp){}
	virtual void Init(const char* szIp){}
	virtual void Close(){}
	virtual void Connect(){}
	virtual bool DisConnect(){ return false;}
	virtual bool HandUserMsg(int nEvent, void* pData, USHORT uDataSize) {return false;}
	virtual bool HandNetData(USHORT, USHORT, USHORT, void*, USHORT){return true;}
};


