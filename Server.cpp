#include <assert.h>
#include "types.h"
#include "Server.h"
#include "Core.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "RoomManager.h"
#include "Room.h"
#include "MemDataBaseEnger.h"
#include "DataBaseEnginer.h"

USHORT g_serno = -1;
USHORT g_nSerType = 1;
UINT g_gameid = 0;

static CRoomManager* m_pRoomManager = NULL;
static CMemDataBaseEnginer* m_pMem = NULL;
static CDataBaseEnginer* m_pDataBase = NULL;

CServer::CServer():m_nDeamon(0)
{
	m_pCore = CCore::CreateSingle();
	int nCpuNum = sysconf(_SC_NPROCESSORS_ONLN);
	m_pCore->SetThreadNum(nCpuNum*2);
}

void CServer::InitConnServer(char* const pLogFile,USHORT nSerNo,USHORT nPort,USHORT nWebSockPort)
{
	g_serno = nSerNo;
	g_nSerType = 2;
	InitLogFile(pLogFile);
	if(nPort > 0)
		m_pCore->AddNetSer("0.0.0.0",nPort);
	
	if(nWebSockPort > 0)
		m_pCore->AddWebSockSer("0.0.0.0",nWebSockPort);

	ConnectDataServer();
}

void CServer::InitGameServer(char* const pLogFile,USHORT nSerNo,UINT nGameId)
{
	g_serno = nSerNo;
	g_gameid = nGameId;
	g_nSerType = 3;
	InitLogFile(pLogFile);
	
	m_pRoomManager = Single_Create(CRoomManager)  ;
	if(!CRoom::LoadGameLogic(nGameId))
	{
		printf("Load GameLogic %d Failer\n",nGameId);
		exit(0);
	}
	
	ConnectDataServer();
	ConnectConnServer();
}

void CServer::InitDataServer(char* const pLogFile,USHORT nPort)
{
	InitLogFile(pLogFile);
	
	m_pMem = Single_Create(CMemDataBaseEnginer);
	m_pDataBase = Single_Create(CDataBaseEnginer);
	m_pMem->SetServiceNum(8);
	m_pDataBase->SetServiceNum(8);
	if(nPort > 0)
		m_pCore->AddNetSer("0.0.0.0",nPort);
}

void CServer::InitLogFile(char* const pLogFile)
{
	if(strcmp(pLogFile,"0"))
	{
		char szFileName[128] = {0};
		printf("%s/log/%s\n",get_current_dir_name(),pLogFile);
		sprintf(szFileName,"%s/log/%s",get_current_dir_name(),pLogFile);
		m_pCore->InitLogFileName(szFileName);
		m_nDeamon = 1;
	}
}

void CServer::ConnectDataServer()
{
	ifstream in("./config/data.config");
	if (in.is_open())
	{
		char szConfig[64] = { 0 };
		while (in.getline(szConfig, 64))
		{
			char *szIp = strtok(szConfig, ":");
			char *szPort = strtok(NULL, ":");

			if (szIp && szPort)
			{
				for(int i = 0; i < MAX_LINK_DATA; i++)
				{
					m_pCore->AddNetCli(szIp, atoi(szPort));
				}
			}
		}
		in.close();
	}
	else
	{
		printf("not find data.config file\n");
		exit(0);
	}
}

void CServer::ConnectConnServer()
{
	ifstream in("./config/conn.config");
	if (in.is_open())
	{
		char szConfig[64] = { 0 };
		while (in.getline(szConfig, 64))
		{
			char *szIp = strtok(szConfig, ":");
			char *szPort = strtok(NULL, ":");

			if (szIp && szPort)
			{
				for(int i = 0; i < MAX_LINK_CONN; i++)
				{
					m_pCore->AddNetCli(szIp, atoi(szPort));
				}
			}
		}
		in.close();
	}
	else
	{
		printf("not find conn.config file\n");
		exit(0);
	}
}

CServer::~CServer()
{
	m_pCore->Destroy();
	Single_Destroy(CRoomManager);
	Single_Destroy(CDataBaseEnginer);
	Single_Destroy(CMemDataBaseEnginer);
	CRoom::ReleaseGameLogic();
}
void CServer::Run()
{	
	if(m_nDeamon)
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
	
	m_pCore->Run();
}