#pragma once
#include "types.h"
#include <assert.h>

class CServices;
class CSocketService;

class CCore
{
	Single(CCore)
public:
	//运行服务
	void Run();
	
	//程序中止
	void Stop();
public:
	//设置服务线程数 默认为1
	void SetThreadNum(int nNum = 1);

	//添加一个服务 服务必须继承自 CServices
	bool AddService(CServices* pServices);

	//设置最大的消息包 不设置的话系统默认为2048
	void SetMaxMsgSize(int nMaxSize);
	
	//添加一个listen 套接字接收连接
	void AddNetSer(unsigned short nPort);
	
	//添加普通连接套接字
	void AddNetCli(char* szIp,unsigned short nPort);
	
	//添加Http服务
	void AddHttpSer(unsigned short nPort);
	
	//添加websocket服务
	void AddWebSockSer(unsigned short nPort);

	//网络引擎添加自定义套接字 自定义套接字必须继承自CSocketService
	void AddSocket(CSocketService* pSocketService,UINT events);
	
	//网络引擎修改自定义套接字
	void ModSocketWrite(CSocketService* pSocketService, UINT events);
	
	//网络引擎删除自定义套接字
	void CloseSocket(CSocketService* pSocketService);
	
	//设置日志输入文件，如果不设置则默认输出到标准输出
	void InitLogFileName(char* const pFileName);
};

