#pragma once

#include <string.h>

typedef unsigned long long UINT64;
typedef unsigned short USHORT;
typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef unsigned int UID;

typedef unsigned short TIMEERID;
typedef unsigned short SERVICEINDEX;
typedef unsigned DATASIZE;
#define MAX_SERVICE_NUM 0xFFFF
#define INVALID_SERIVCE_INDEX 0

#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))
#define SAFE_DELTE(p) if(p){delete p;}
#define SAFE_DELTEARRAY(p) if(p){delete[] p;}

#define MAX_MSG_SIZE 2048
#define MAX_SER_NUM 100
#define MAX_GAME_SER_NUM 100
#define MAX_LINK_DATA 4
#define MAX_LINK_CONN 32


#define Single(T) \
private:\
	T();\
	~T();\
	T(const T& t){}\
	T& operator=(const T& t){ return *this;}\
	static T* m_sSingle;\
public:\
	static T* CreateSingle(){assert(m_sSingle == NULL);m_sSingle = new T; return m_sSingle;}\
	static T* GetInstance(){return m_sSingle;}\
	void Destroy(){delete this; T::m_sSingle = NULL;}

#define Single_Init(T) \
	T* T::m_sSingle = NULL;

#define Single_Create(T) \
	T::CreateSingle()

#define Single_Get(T) \
	T::GetInstance()

#define Single_Destroy(T) \
	if(T::GetInstance())\
	{\
		T::GetInstance()->Destroy();\
	}

enum TYPE_EVENT
{
	EXIT_MSG = 0,		//退出消息
	LOG_MSG,			//写日志
	NET_MSG,			//网络消息
	NET_CONNECT,		//连接成功
	NET_RECONNECT, 		//重连消息
	SEND_DATA_REQ,		//发送数据请求
	TIME_MSG,			//时间消息
	USER_MSG,			//用户消息
	DATA_BASE_REQ,		//数据库请求
	DATA_BASE_RET,		//数据库结果
	MEM_DATA_BASE_REQ,	//缓存请求
	MEM_DATA_BASE_RET,	//缓存结果
	EN_DOUBLE_LOGIN,
	EN_SYNC_GAME_SER_INFO,
	EN_RECONNECT_FAIL,
	MAX_MSG
};

/*
enum EN_USER_MSG
{
	EN_DOUBLE_LOGIN=1,
	EN_SYNC_GAME_SER_INFO,
	EN_RECONNECT_FAIL,
};*/

typedef struct tagNetHead
{
	USHORT uDataSize;
}NetHead;

typedef struct tagNetMsgCommand
{
	USHORT uMain;
	USHORT uSub;
}NetMsgCommand;

typedef struct tagUser2Room
{
	USHORT nIndex;
	USHORT nMain;
	USHORT nSub;
}User2Room;

typedef struct tagUser2Game
{
	USHORT nIndex;
	USHORT nMain;
	USHORT nSub;
	int nSeatNo;
}User2Game;

typedef struct tagGame2User
{
	USHORT nIndex;
	USHORT nMain;
	USHORT nSub;
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
	USHORT uSeatId;

	tagUser()
	{
		memset(this, 0, sizeof(tagUser));
	}
} Users;

enum Main_MSG
{
	MAIN_MSG_MIN = 0,
	MAIN_MSG_NET,
	MAIN_MSG_LOGIN,
	MAIN_MSG_LOBBY,
	
	MAIN_MSG_ROOM_MANAGER,
	MAIN_MSG_ROOM,
	MAIN_MSG_GAME,

	MAIN_MSG_GAMESER,
	MAIN_MSG_CONNECT,

	MAIN_MSG_MEM,
	MAIN_MSG_DATA,

	MAIN_MSG_DATACENTER, 	//数据中心消息
	MAIN_ERROR,
	MAIN_MSG_MAX
};

enum MAIN_MSG
{
	MAIN_MSG_CENTERSER=1,
	MAIN_MSG_USERSER,
	MAIN_MSG_DATASER,
	MAIN_MSG_GAMESER,
	MAIN_MSG_CONNSER,
	MAIN_MSG_USER,
	MAIN_MSG_MAX,
};

enum Sub_Msg_Center
{
	CEN_SUB_MSG_CONN_SUCSS = 1,
	CEN_SUB_MSG_NEWGAMESER = 2,
};

enum Sub_Msg_UserServer
{
	US_SUB_MSG_CONN_SUCSS = 1,
};

enum Sub_Msg_DataSer
{
	DS_SUB_MSG_CONN_SUCSS = 1,
};

//MAIN_MSG_CONNECT
#define SUB_MSG_USER4ROOM 1
#define SUB_MSG_USER4GAME 2
#define SUB_MSG_USER_DOUBLELOGIN 3
#define SUB_MSG_USER_RELOGIN 4

//MAIN_MSG_GAMESER
#define SUB_MSG_GAME4USER 1
#define SUB_MSG_GAME2CONN 2
#define SUB_MSG_RECONNECT_FAIL 3

//MAIN_MSG_NET
enum MAIN_MSG_NET
{
	SUB_MSG_CONN_SUCSS = 1,
	SUB_MSG_TEST,
	SUB_MSG_REG_USERSER,
	SUB_MSG_REG_DATASER,
	SUB_MSG_REG_GAMESER,
	SUB_MSG_REG_CONNECTSER,
	
	SUB_MSG_DATA_SUCSS,
};

//define MAIN_MSG_LOGIN
#define SUB_MSG_LOGIN 1
#define SUB_MSG_REGIST 2
#define SUB_MSG_DOUBLE_LOGIN 3


//define MAIN_MSG_LOBBY

//MAIN_MSG_DATACENTER
#define SUB_MSG_DATA_BASE_REQ 1		//数据库请求
#define SUB_MSG_DATA_BASE_RET 2		//数据库结果
#define SUB_MSG_MEM_DATA_BASE_REQ 3	//缓存请求
#define SUB_MSG_MEM_DATA_BASE_RET 4	//缓存结果


//define MAIN_MSG_ROOM_MANAGER
#define SUB_MSG_CREATE 1
#define SUB_MSG_ENTER 2
#define SUB_MSG_CREATE_FAIL 3
#define SUB_MSG_JOIN_FAIL 4

//MAIN_MSG_ROOM
#define SUB_MSG_ROOMINFO 1
#define SUB_MSG_JOIN 2
#define SUB_MSG_USER_RECONNECT 3
#define SUB_MSG_USER_DISCONNECT 4
#define SUB_MSG_USER_READY 5
#define SUB_MSG_ALLUSERINFO 6


//MAIN_ERROR
#define MAIN_USER_MSG_ERROR 1

//定时器
#define TIME_CONN_RECONNECT 1
#define TIME_CONN_IS_LINK 2
