#pragma once
#include "types.h"

class CServices 
{
public:
	CServices();
	virtual ~CServices();
protected:
	/*
		Activated 代表该服务成为一个合法服务
		注意所有与 GetServiceIndex() 相关的数据设置初始化请在 Activated 函数中进行
		定时器不能在构造函数中启动，请在 Activated 函数中启动 
		如有需要请在子类重写
	*/
	virtual void Activated();
	
	/*
		返回值：false 服务会退出 true 正常处理
		函数参数：
		nType : 消息类型
		nSrcIndex : 此消息由哪个服务投递
		pData : 消息体数据指针
		nSize : 消息体大小
	*/
	virtual bool HandData(int nType, SERVICEINDEX nSrcIndex, void *pData, DATASIZE nSize) = 0;
	
	/*
		服务退出处理，默认是释放该服务内存
		如果改用内存池请在子类重写此函数
	*/
	virtual void ExitSelf();
public:
	//获取当前服务索引函数
	SERVICEINDEX GetServiceIndex() const;
	//消息投递函数
	bool PostData(SERVICEINDEX nDstIndex,int nType, void* pData = 0, DATASIZE size = 0);
	//日志输出
	void Log(const char* szFormat,...);
private:
	void* p;
};