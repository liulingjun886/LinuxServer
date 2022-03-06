#pragma once
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include "Services.h"
#include "ToolLock.h"

class CNetSink;
typedef CNetSink* (*FucCreateNetSink)(CServices* pServices);

class CPoller:
	public CServices
{
public:
	CPoller();
	~CPoller();
public:
	bool SetNoBlock();
	int GetFd() const;
protected:
	int m_nPollId;
};
