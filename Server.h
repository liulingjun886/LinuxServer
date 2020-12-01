#pragma once
#include <string>
#include "./include/Core.h"
#include "Defines.h"

class CServer
{
public:
	CServer();
	virtual ~CServer();
	int	 Init(unsigned short nSerType,unsigned short nSerNo);
	void Run();
public:
	const char* GetIP() const;
	unsigned short GetPort() const;
	uint16 GetSerType() const;
	uint16 GetSerNo() const;
protected:
	void InitLogFile(const char*   pLogFile);
	virtual int Initialize() = 0;
private:
	void SetDaemon();
protected:
	CCore* m_pCore;
	int m_nDeamon;
	unsigned short m_nSerType;
	unsigned short m_nSerNo;
};
