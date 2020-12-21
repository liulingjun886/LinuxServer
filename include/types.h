#pragma once

#include <string.h>

typedef unsigned long long uint64;
typedef long long int64;

typedef unsigned int uint32;
typedef int int32;

typedef unsigned short uint16;
typedef short int16;

typedef char int8;
typedef unsigned char uint8;

#define MAX_SERVICE_NUM 0xFFFF
#define INVALID_SERIVCE_INDEX 0

#define MAX_LOG_SIZE 512

typedef uint16 TIMEERID;
typedef uint16 SERVICEINDEX;
typedef uint32 DATASIZE;

#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))
#define SAFE_DELTE(p) if(p){delete p;}
#define SAFE_DELTEARRAY(p) if(p){delete[] p;}

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
