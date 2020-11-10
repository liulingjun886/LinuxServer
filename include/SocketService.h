#pragma once
#include "Services.h"
#include "ToolLock.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>



class CSocketService:
	public CServices
{
public:
	CSocketService();
	~CSocketService();
public:
	static void SetNoBlock(int fd);
	int GetFd() const;
protected:
	int m_sockfd;
};
