#include <assert.h>
#include "./Defines.h"
#include "Server.h"
#include "./include/core/Core.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "./include/core/SingleObject.h"

CServer::CServer():m_nDeamon(0)
{
	//m_pCore = CCore::CreateSingle();
}

int	 CServer::Init(unsigned short nSerType,unsigned short nSerNo)
{
	if(nSerNo >= 500 || nSerNo < 0)
		return -1;
	m_nSerType = nSerType;
	m_nSerNo = nSerNo;
	
	int nCpuNum = sysconf(_SC_NPROCESSORS_ONLN);
	CSingleObject<CCore>::Instance()->SetThreadNum(nCpuNum*2);
	
	return Initialize();
}

void CServer::InitLogFile(const char*   pLogFile)
{
	if(strcmp(pLogFile,"0"))
	{
		char szFileName[128] = {0};
		char* szDir = get_current_dir_name();
		printf("%s/log/%s\n",szDir,pLogFile);
		sprintf(szFileName,"%s/log/%s",szDir,pLogFile);
		free(szDir);
		//m_pCore->InitLogFileName(szFileName);
		CSingleObject<CCore>::Instance()->InitLogFileName(szFileName);
		m_nDeamon = 1;
	}
}

void CServer::SetDaemon()
{
	int pid, null;
	char nullbuf[] = {0x2f,0x64,0x65,0x76,0x2f,0x6e,0x75,0x6c,0x6c,0x00};

	pid = fork();
	if(pid < 0)
	{
		printf("Create service failer!\n");
		exit(0);
	}

	if(pid > 0)
	{
		exit(0);
	}

	setsid();
	//chdir ("/");
	null = open (nullbuf, O_RDWR);
	dup2 (null, 0);
	dup2 (null, 1);
	dup2 (null, 2);
	close (null);
}

CServer::~CServer()
{
	//CSingleObject<CCore>::Instance()->Destroy();
}
void CServer::Run()
{	
	if(1 == m_nDeamon)
		SetDaemon();

	srand(time(NULL));
	CSingleObject<CCore>::Instance()->Run();
}

uint16 CServer::GetSerType() const
{
	return m_nSerType;
}

uint16 CServer::GetSerNo() const
{
	return m_nSerNo;
}