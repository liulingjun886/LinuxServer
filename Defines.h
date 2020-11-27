#pragma once

#include "./include/types.h"
#include <string>

#define MAX_MSG_SIZE 2048

#define MAX_USER_DATA_SRV_NUM 16
#define MAX_CONN_GAME_SRV_NUM 500
#define MAX_CONN_TO_GAME_LINK 31

enum USER_EVENT
{
	USER_MSG = MAX_SYS_MSG,	//用户消息
	DATA_BASE_REQ,			//数据库请求
	DATA_BASE_RET,			//数据库结果
	MEM_DATA_BASE_REQ,		//缓存请求
	MEM_DATA_BASE_RET,		//缓存结果
	EN_DOUBLE_LOGIN,
	EN_SYNC_GAME_SER_INFO,
	EN_RECONNECT_FAIL,
	MAX_MSG
};

enum SERVER_TYPE
{
	SRV_TYPE_CENTER = 1,
	SRV_TYPE_USER,
	SRV_TYPE_DATA,
	SRV_TYPE_GAME,
	SRV_TYPE_CONN,
};

enum MAIN_MSG
{
	MAIN_MSG_CENTERSER = 1,
	MAIN_MSG_USERSER,
	MAIN_MSG_DATASER,
	MAIN_MSG_GAMESER,
	MAIN_MSG_CONNSER,
	MAIN_MSG_HALL,
	MAIN_MSG_ROOM,
	MAIN_MSG_GAME,
	MAIN_MSG_MAX,
};

enum Sub_Msg_CenterSer
{
	CT_SUB_MSG_CONN_SUCSS = 1,
	CT_SUB_MSG_TEST,
	CT_SUB_MSG_NEWGAMESER,
};

enum Sub_Msg_UserSer
{
	US_SUB_MSG_CONN_SUCSS = 1,
	US_SUB_MSG_TEST,
	US_SUB_MSG_REGUSERSRV,
	US_SUB_MSG_DATA_BASE_RET,
};

enum Sub_Msg_DataSer
{
	DS_SUB_MSG_CONN_SUCSS = 1,
	DS_SUB_MSG_TEST,
	DS_SUB_MSG_REG_DATASRV,
};

enum Sub_Msg_GameSer
{
	GS_SUB_MSG_CONN_SUCSS = 1,
	GS_SUB_MSG_TEST,
	GS_SUB_MSG_REG_GAMESRV,
	GS_SUB_MSG_GAME2USER,
	GS_SUB_MSG_GAME2CONN,
};

enum Sub_Msg_ConnSer
{
	CS_SUB_MSG_CONN_SUCSS = 1,
	CS_SUB_MSG_TEST,
	CS_SUB_MSG_REG_CONN,
};

typedef int32 UID;

typedef struct tagUser2Room
{
	uint16 nIndex;
	uint16 nMain;
	uint16 nSub;
}User2Room;

typedef struct tagUser2Game
{
	uint16 nIndex;
	uint16 nMain;
	uint16 nSub;
	int nSeatNo;
}User2Game;

typedef struct tagGame2User
{
	uint16 nIndex;
	uint16 nMain;
	uint16 nSub;
}Game2User;

typedef struct tagDataCenter
{
	SERVICEINDEX nCsid;
}DataCenter;

typedef struct tagUser
{
	UID iUserId;
	char szName[32];
	char bSex;
	char szHeadUrl[128];
	unsigned int nDiamond;
	unsigned int nJinBi;
	unsigned int nSockIndex;
	int nConnSer;
	int nGameSer;
	int nRoomId;
	uint16 uSeatId;

	tagUser()
	{
		memset(this, 0, sizeof(tagUser));
	}
} Users;


struct HostConfig
{
	std::string szIp;
	uint16 nPort;
};

struct GameInfo : public HostConfig
{
	int 	nGameId;
	int 	nGameNo;
	
};
