#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "Server.h"


int main(int argc,char* argv[])
{
	if (argc != 5 && argc != 6)
	{
		printf("use ./server nSerType nSerNo pLogFileName [nPort/nGameId] [nWebSockPort]!\n");
		return 0;
	}

	CServer* pSer =  new CServer;
	USHORT nSerType = (USHORT)atoi(argv[1]);
	USHORT nSerNo = (USHORT)atoi(argv[2]);
	char *szLogFile = argv[3];
	switch (nSerType)
	{
		case 1:
		{
			USHORT nPort = (USHORT)atoi(argv[4]);
			pSer->InitDataServer(szLogFile,nPort);
			break;
		}
		case 3:
		{
			USHORT nGameId = (USHORT)atoi(argv[4]);
			pSer->InitGameServer(szLogFile,nSerNo,  nGameId);
			break;
		}
		case 2:
		{
			USHORT nPort = (USHORT)atoi(argv[4]);
			USHORT nWebSocketPort = 0;
			if(argc == 6)
				nWebSocketPort = (USHORT)atoi(argv[5]);
			pSer->InitConnServer(szLogFile,nSerNo, nPort, nWebSocketPort);
			break;
		}
		default:
		{
			printf("nSerType fault!\n");
			return 0;
		}
	}
	
	pSer->Run();
	delete pSer;
	return 0;
}
