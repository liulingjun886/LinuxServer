#pragma once
#include "Defines.h"
#include <string>

typedef struct tagNetHead
{
	uint32 uDataSize;
}NetHead;

typedef struct tagNetMsgCommand
{
	uint16 uMain;
	uint16 uSub;
}NetMsgCommand;


uint16 nHeadSize = sizeof(NetHead);
uint16 nCommandSize = sizeof(NetMsgCommand);
uint16 nMinDataSize = nHeadSize + nCommandSize;

class CPacketBase
{
public:
	CPacketBase();
	~CPacketBase();
public:
	char* data();
	uint32 data_len();
protected:
	bool _Copy(const void* pBuf, uint32 nLen);
	void _Begin(uint16 nMain, uint16 nSub);
	void _End();
	void _Reset();
	bool _Read(void* pBuf, uint32 nLen);
	char* _ReadPoint(uint32 nLen);
	bool _Write(const void* pBuf, uint32 nLen);
	bool _ReadHeader(void* pBuf, uint32 nLen, uint32 nPos);
	bool _WriteHeader(void* pBuf, uint32 nLen, uint32 nPos);
private:
	char m_szBuf[MAX_MSG_SIZE];
	uint32 m_nPacketSize;
	uint32 m_nCurrPos;
};


class CInputPacket : public CPacketBase
{
public:
	CInputPacket();
	~CInputPacket();
public:
	static int32 GetPacketLen(const char* pData, uint32 nDataLen);
public:
	bool Copy(const void* pBuf, uint32 nLen);
	uint16 GetMainCmd();
	uint16 GetSubCmd();
	int8 ReadInt8();
	int16 ReadInt16();
	int32 ReadInt32();
	int64 ReadInt64();
	float ReadFloat();
	double ReadDouble();
	const char* ReadString();
};



class COutputPacket : public CPacketBase
{
public:
	COutputPacket();
	~COutputPacket();
public:
	void Begin(uint16 nMain, uint16 nSub);
	void End();
	bool WriteInt8(uint8 nValue);
	bool WriteInt16(uint16 nValue);
	bool WriteInt32(uint32 nValue);
	bool WriteInt64(uint64 nValue);
	bool WriteFloat(float nValue);
	bool WriteDouble(double nValue);
	bool WriteString(const char* szValue);
	bool WriteString(const std::string& szValue);
};