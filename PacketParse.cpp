#include "PacketParse.h"
#include <string.h>
#include <assert.h>

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


#define HEAD_SIZE nMinDataSize


CPacketBase::CPacketBase():m_pData(NULL)
{
	
}

CPacketBase::~CPacketBase()
{
	
}

char* CPacketBase::Get_Packet()
{
	return m_pData;
}

uint32 CPacketBase::Packet_Len()
{
	return m_nPacketSize;
}


char* 	CPacketBase::RestPacket()
{
	return &m_pData[m_nCurrPos];
}

uint32	CPacketBase::Rest_Len()
{
	return m_nPacketSize - m_nCurrPos;
}

char* 	CPacketBase::Data()
{
	return &m_pData[HEAD_SIZE];
}

uint32 	CPacketBase::Data_Len()
{
	return m_nPacketSize - HEAD_SIZE;
}


bool CPacketBase::_Copy(const void* pBuf, uint32 nLen, bool bHead)
{
	if(nLen > MAX_MSG_SIZE)
		return false;
	_Reset();
	m_pData = (char*)pBuf;
	m_nPacketSize = nLen;
	if(!bHead)
		m_nCurrPos = 0;
	return true;
}
void CPacketBase::_Begin(uint16 nMain, uint16 nSub)
{
	_Reset();
	_WriteHeader(&nMain, sizeof(uint16), 4);
	_WriteHeader(&nSub, sizeof(uint16), 6);
}

void CPacketBase::_End()
{
	_WriteHeader(&m_nCurrPos, (uint32)sizeof(uint32), 0);
	m_nPacketSize = m_nCurrPos;
}

void CPacketBase::_Reset()
{
	m_nCurrPos = HEAD_SIZE;
	m_nPacketSize = HEAD_SIZE;
}


bool CPacketBase::_Read(void* pBuf, uint32 nLen)
{
	if(m_nCurrPos+nLen > m_nPacketSize)
		return false;

	memcpy(pBuf, &m_pData[m_nCurrPos], nLen);
	m_nCurrPos += nLen;
	return true;
}

char* CPacketBase::_ReadString()
{
	uint32 nLen = (uint32)strlen(&m_pData[m_nCurrPos]);
	return _ReadPoint(nLen + 1);
}


char* CPacketBase::_ReadPoint(uint32 nLen)
{
	if(m_nCurrPos+nLen > m_nPacketSize)
		return NULL;
	
	if(0 == nLen)
		return NULL;
	
	char* pTemp = &m_pData[m_nCurrPos];
	m_nCurrPos += nLen;
	return pTemp;
}


bool CPacketBase::_Write(const void* pBuf, uint32 nLen)
{
	if(m_nCurrPos+nLen > MAX_MSG_SIZE)
		return false;

	memcpy(&m_pData[m_nCurrPos], pBuf, nLen);
	m_nCurrPos += nLen;
	return true;
}

bool CPacketBase::_ReadHeader(void* pBuf, uint32 nLen, uint32 nPos)
{
	if(nPos + nLen > HEAD_SIZE)
		return false;
	memcpy(pBuf, &m_pData[nPos], nLen);
	return true;
}

bool CPacketBase::_WriteHeader(void* pBuf, uint32 nLen, uint32 nPos)
{
	if(nPos + nLen > HEAD_SIZE)
		return false;
	memcpy(&m_pData[nPos], pBuf, nLen);
	return true;
}






CInputPacket::CInputPacket()
{
	
}
CInputPacket::~CInputPacket()
{
	
}

int32 CInputPacket::GetPacketLen(const char* pData, uint32 nDataLen)
{
	if(nDataLen < nMinDataSize)
		return 0;

	NetHead* pHead = (NetHead*)pData;
	int32 nPacketSize = (int32)pHead->uDataSize;

	if(nPacketSize > MAX_MSG_SIZE)
		return -1;
	
	if(nDataLen < nPacketSize)
		return 0;

	return nPacketSize;
}


bool CInputPacket::Copy(const void* pBuf, uint32 nLen, bool bHead)
{
	assert(NULL != pBuf);
	return CPacketBase::_Copy(pBuf, nLen, bHead);
}

uint16 CInputPacket::GetMainCmd()
{
	uint16 nMain = 0;
	_ReadHeader(&nMain, sizeof(uint16), 4);
	return nMain;
}

uint16 CInputPacket::GetSubCmd()
{
	uint16 nSub = 0;
	_ReadHeader(&nSub, sizeof(uint16), 6);
	return nSub;
}

int8 CInputPacket::ReadInt8()
{
	int8 nValue = 0;
	CPacketBase::_Read(&nValue, sizeof(int8));
	return nValue;
}

int16 CInputPacket::ReadInt16()
{
	int16 nValue = 0;
	CPacketBase::_Read(&nValue, sizeof(int16));
	return nValue;
}

int32 CInputPacket::ReadInt32()
{
	int32 nValue = 0;
	CPacketBase::_Read(&nValue, sizeof(int32));
	return nValue;
}

int64 CInputPacket::ReadInt64()
{
	int64 nValue = 0;
	CPacketBase::_Read(&nValue, sizeof(int64));
	return nValue;
}

float CInputPacket::ReadFloat()
{
	float nValue = 0.f;
	CPacketBase::_Read(&nValue, sizeof(float));
	return nValue;
}

double CInputPacket::ReadDouble()
{
	double nValue = 0;
	CPacketBase::_Read(&nValue, sizeof(double));
	return nValue;
}

const char* CInputPacket::ReadString()
{
	return ReadBinary();
}

const char* CInputPacket::ReadBinary()
{
	uint32 nLen = 0;
	CPacketBase::_Read(&nLen,sizeof(uint32));
	return ReadBinary(nLen);
}


const char* CInputPacket::ReadBinary(uint32 nLen)
{
	if(0 == nLen)
		return NULL;
	
	return CPacketBase::_ReadPoint(nLen);
}






COutputPacket::COutputPacket()
{
	m_pData = &m_szBuf[0];
}

COutputPacket::~COutputPacket()
{
	
}

void COutputPacket::Begin(uint16 nMain, uint16 nSub)
{
	memset(m_szBuf,0,sizeof(m_szBuf));
	CPacketBase::_Begin(nMain, nSub);
}

void COutputPacket::End()
{
	CPacketBase::_End();
}

bool COutputPacket::WriteInt8(uint8 nValue)
{
	return CPacketBase::_Write(&nValue, sizeof(uint8));
}

bool COutputPacket::WriteInt16(uint16 nValue)
{
	return CPacketBase::_Write(&nValue, sizeof(uint16));
}

bool COutputPacket::WriteInt32(uint32 nValue)
{
	return CPacketBase::_Write(&nValue, sizeof(uint32));
}

bool COutputPacket::WriteInt64(uint64 nValue)
{
	return CPacketBase::_Write(&nValue, sizeof(uint64));
}

bool COutputPacket::WriteFloat(float nValue)
{
	return CPacketBase::_Write(&nValue, sizeof(float));
}

bool COutputPacket::WriteDouble(double nValue)
{
	return CPacketBase::_Write(&nValue, sizeof(double));
}

bool COutputPacket::WriteString(const char* szValue)
{

	if(NULL == szValue)
		szValue = "";
	
	uint32 nLen = strlen(szValue)+1;
	return WriteBinary(szValue, nLen);
}

bool COutputPacket::WriteString(const std::string& szValue)
{
	return WriteString(szValue.c_str());
}

bool COutputPacket::WriteBinary(const void* pData, uint32 nLen)
{
	if(NULL == pData)
		return false;
	
	return CPacketBase::_Write(&nLen, sizeof(uint32)) && CPacketBase::_Write(pData, nLen);
}

