#pragma once
#include "types.h"

struct RegGameSer
{
	USHORT nSerNo;
	UINT nGameID;
	char szIp[16];
	USHORT nPort;
	RegGameSer()
	{
		memset(this,0,sizeof(RegGameSer));
	}
};

struct RegConnSer
{
	USHORT nSerNo;
};

typedef struct tagCreateRoomInfo
{
	UINT nGameId;
	USHORT uPlayerCount;
	USHORT uRound;
}CreateRoomInfo;

typedef struct tagLogicInfo
{
	USHORT nNum;
	USHORT nRules[1];
}LogicInfo;

typedef struct tagCreateRoom
{
	CreateRoomInfo roominfo;
	LogicInfo logicinfo;
}CreateRoom;


typedef struct tagJoinRoom
{
	UINT nRoomId;
}JoinRoom;


typedef struct tagRoomInfo
{
	UINT	nGameId;
	UINT 	nRoomId;
	USHORT 	nRound;
	USHORT 	nUserCount;
	USHORT	nCurRound;
}RoomInfo;

typedef struct tagUserDoubleLogin
{
	UID nUserId;
	USHORT nCid;
	USHORT nCsid;
}UserDoubleLogin;

typedef struct tagUserGameSerInfo
{
	UINT nUserId;
	USHORT nGid;
	USHORT nGsid;
	USHORT nSeatNo;
}UserGameSerInfo;

typedef struct tagInnerSync
{
	USHORT nIndex;
	UINT nType;
}InnserSync;

typedef struct tagUserConnSerInfo
{
	USHORT nCid;
	USHORT nCsid;
}UserConnSerInfo;

typedef struct tagUserReConnInfo
{
	UID nUserId;
	USHORT nCid;
	USHORT nCsid;
}UserReConnInfo;