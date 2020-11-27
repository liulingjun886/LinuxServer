#pragma once
#include "./Defines.h"

struct ConnSucess
{
	uint16 nSrvType;
	uint16 nSrvNo;
};

struct RegGameSer
{
	uint16 nSerNo;
	uint32 nGameID;
	char szIp[16];
	uint16 nPort;
	RegGameSer()
	{
		memset(this,0,sizeof(RegGameSer));
	}
};

struct RegConnSer
{
	uint16 nSerNo;
};

typedef struct tagCreateRoomInfo
{
	uint32 nGameId;
	uint16 uPlayerCount;
	uint16 uRound;
}CreateRoomInfo;

typedef struct tagLogicInfo
{
	uint16 nNum;
	uint16 nRules[1];
}LogicInfo;

typedef struct tagCreateRoom
{
	CreateRoomInfo roominfo;
	LogicInfo logicinfo;
}CreateRoom;


typedef struct tagJoinRoom
{
	uint32 nRoomId;
}JoinRoom;


typedef struct tagRoomInfo
{
	uint32	nGameId;
	uint32 	nRoomId;
	uint16 	nRound;
	uint16 	nUserCount;
	uint16	nCurRound;
}RoomInfo;

typedef struct tagUserDoubleLogin
{
	UID nUserId;
	uint16 nCid;
	uint16 nCsid;
}UserDoubleLogin;

typedef struct tagUserGameSerInfo
{
	UID nUserId;
	uint16 nGid;
	uint16 nGsid;
	uint16 nSeatNo;
}UserGameSerInfo;

typedef struct tagInnerSync
{
	uint16 nIndex;
	uint32 nType;
}InnserSync;

typedef struct tagUserConnSerInfo
{
	uint16 nCid;
	uint16 nCsid;
}UserConnSerInfo;

typedef struct tagUserReConnInfo
{
	UID nUserId;
	uint16 nCid;
	uint16 nCsid;
}UserReConnInfo;