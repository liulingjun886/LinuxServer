#pragma once
#include "Services.h"
#include "ToolLock.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>



class CPoller:
	public CServices
{
public:
	CPoller();
	~CPoller();
public:
	void SetNoBlock();
	int GetFd() const;
protected:
	int m_nPollId;
};
