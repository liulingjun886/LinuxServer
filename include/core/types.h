#pragma once

typedef unsigned long long uint64;
typedef long long int64;

typedef unsigned int uint32;
typedef int int32;

typedef unsigned short uint16;
typedef short int16;

typedef char int8;
typedef unsigned char uint8;

#define MAX_SERVICE_NUM 65536
#define INVALID_SERIVCE_INDEX 0

#define MAX_LOG_SIZE 512

typedef uint16 TIMEERID;
typedef uint16 SERVICEINDEX;
typedef uint32 DATASIZE;

#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))
#define SAFE_DELTE(p) if(p){delete p;}
#define SAFE_DELTEARRAY(p) if(p){delete[] p;}

enum SYS_EVENTS
{
	EXIT_MSG = 0,		//退出消息
	LOG_MSG,			//写日志
	NET_MSG,			//网络消息
	NET_CONNECT,		//连接成功
	TIME_MSG,			//时间消息
	NET_RECONNECT, 		//重连消息
	SEND_DATA_REQ,		//发送数据请求
	MAX_SYS_MSG,		//用户消息
};
