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

	switch(nSrvNo)
	{
		case SRV_TYPE_CENTER:
			g_pSer = new CCenterServer;
			break;
		
	}

	if(0 != g_pSer->Init(nSrvType, nSrvNo))
		return 0;

	g_pSer->Run();
	delete g_pSer;
	return 0;
}
