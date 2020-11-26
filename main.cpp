#include <stdlib.h>
#include <stdio.h>
#include "./include/types.h"
#include "Server.h"
#include <iostream>
#include "./CenterServer/CenterServer.h"
#include "./UserServer/UserServer.h"
#include "./DataServer/DataServer.h"
#include "./GameServer/GameServer.h"
#include "./ConntionServer/ConnectServer.h"

CServer* g_pSer = NULL;


int main(int argc,char* argv[])
{
	USHORT nSrvType;
	USHORT nSrvNo;
	
	char option[] = ":s:t::";
    int result;
	int num = 0;
    while((result = getopt(argc, argv, option)) != -1)
    {
        switch(result)
        {

            case 's':       //server Id
                nSrvNo = (unsigned short)atoi(optarg);
                if (nSrvNo <= 0)
                {
                    printf("sid=%d\n", nSrvNo);
                    return -1;
                }
				num++;
                break;
	     	case 't':       //server type
                nSrvType= (unsigned short)atoi(optarg);
                if (nSrvType <= 0)
                {
                    printf("m_nLevel=%d\n", nSrvType);
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

	switch(nSrvType)
	{
		case SRV_TYPE_CENTER:
			g_pSer = new CCenterServer;
			break;
		case SRV_TYPE_USER:
			g_pSer = new CUserServer;
			break;
		case SRV_TYPE_DATA:
			g_pSer = new CDataServer;
			break;
		case SRV_TYPE_GAME:
			g_pSer = new CGameServer;
			break;
		case SRV_TYPE_CONN:
			g_pSer = new CConnectServer;
			break;
		default:
			break;
	}

	if(NULL == g_pSer)
	{
		printf("invalid argvs \n");
        return -1;
	}

	if(0 != g_pSer->Init(nSrvType, nSrvNo))
		return 0;

	g_pSer->Run();
	delete g_pSer;
	return 0;
}
