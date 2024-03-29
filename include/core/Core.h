#pragma once
#include "types.h"
#include <assert.h>
#include "Poller.h"


class CCore
{
public:
	CCore();
	~CCore();
	//运行服务
	void Run();
	
	//程序中止
	void Stop();
public:
	//设置服务线程数 默认为1
	void SetThreadNum(int nNum = 1);

	//添加一个服务 服务必须继承自 CServices
	bool AddService(CServices* pServices);

	//获取当前服务数量
	uint32 GetServiceNum();
	
	//添加一个listen 套接字接收连接
	SERVICEINDEX AddTcpNetSer(const char* szIp, unsigned short nPort, FucCreateNetSink pCreateNetSink, bool IsIpV6 = false);
	
	//添加普通连接套接字
	SERVICEINDEX AddTcpNetCli(const char* szIp, unsigned short nPort, FucCreateNetSink pCreateNetSink, bool IsIpV6 = false);

	//添加websocket服务器
	SERVICEINDEX AddWebSockSer(const char* szIp, unsigned short nPort, FucCreateNetSink pCreateNetSink, bool IsIpV6 = false);

	//添加一个UDP套接字
	SERVICEINDEX AddUdpNet(const char* szIp, unsigned short nPort, FucCreateNetSink pCreateNetSink, bool IsIpV6 = false);

	//网络引擎添加自定义套接字 自定义套接字必须继承自CSocketService
	void AddPoller(CPoller* pPollerService,uint32 events);
	
	//网络引擎修改自定义套接字
	void ModPoller(CPoller* pPollerService, uint32 events);
	
	//网络引擎删除自定义套接字
	void DelPoller(CPoller* pPollerService);
	
	//设置日志输入文件，如果不设置则默认输出到标准输出
	void InitLogFileName(char* const pFileName);

	// 日志输出
	void SysLog(const char* szFormat,...);

	//数据传递
	bool PostSysData(SERVICEINDEX nDstIndex,int nType, void* pData, DATASIZE size);
};

