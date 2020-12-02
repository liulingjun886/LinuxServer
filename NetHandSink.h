#pragma once
#include "./Defines.h"
#include "PacketParse.h"

class CServices;

class CNetHandSink
{
protected:
	CServices* const m_pNet;
public:
	CNetHandSink(CServices* pServices):m_pNet(pServices){};
	virtual ~CNetHandSink(){};
public:
	virtual bool HandTimeMsg(uint16 uTimeID) {return true;}
	virtual bool HandDataBaseRet(uint32 uType,void* pData,uint16 uDataSize){return true;}
	virtual bool HandMemDataRet(uint32 uType, void* pData, uint16 uDataSize){return true;}
	virtual void Init(const char* szIp){}
	virtual void Close(){}
	virtual void Connect(){}
	virtual bool DisConnect(){ return false;}
	virtual bool HandUserMsg(int nEvent, void* pData, uint16 uDataSize) {return false;}
	virtual bool HandNetData(uint16, uint16, uint16, void*, uint32){return true;}
	virtual bool HandNetData(uint16,uint16, CInputPacket& pInPack) {return true;};
};


