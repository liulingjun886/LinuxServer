#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "Server.h"
#include <iostream>
#include "./CenterServer/CenterServer.h"

CServer* g_pSer = NULL;


int main(int argc,char* argv[])
{
	if (argc != 3 && argc != 6)
	{
		printf("use ./server nSerType nSerNo");
		return 0;
	}

	char option[] = ":s:t::";
    int result;
	int num = 0;
    while((result = getopt(argc, argv, option)) != -1)
    {
        switch(result)
        {

            case 's':       //server Id
                g_nSerNo = (unsigned short)atoi(optarg);
                if (g_nSerNo <= 0)
                {
                    printf("sid=%d\n", g_nSerNo);
                    return -1;
                }
				num++;
                break;
	     	case 't':       //server type
                g_nSerType= (unsigned short)atoi(optarg);
                if (g_nSerType <= 0)
                {
                    printf("m_nLevel=%d\n", g_nSerType);
                    return -1;
                }
				num++;
                break;
            case '?':
                printf("Invalid option.\r\n");
                return -1;
            case ':':
                printf("Lack of option argument.\r\n");
                return -1;
        }
    };

	switch(nSerType)
	{
		case 1:
			g_pSer = new CCenterServer;
			break;
		
	}

	if(0 != g_pSer->Init(g_nSerType, g_nSerNo))
		return 0;

	g_pSer->Run();
	delete g_pSer;
	return 0;
	
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
