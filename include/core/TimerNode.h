#pragma once
/*
*
* 如果定时器是判断玩家超时操做，那么在执行定时器函数之前最好校验一下定时器的合法性
* 也就是说用当前时间减去定时器开始时间，避免玩家和定时器一起触发操做
* 另外在服务构造函数中不要调用StartTimer()函数
*/
#include "types.h"

class CServices;

class CTimerNode
{
	friend class CTimerEnginer;
public:
	CTimerNode();
	~CTimerNode();
public:
	//设置定时器绑定的Service
	void InitTimerObj(CServices* pService,uint32 nTimerId);
	//单位是10毫秒
	void StartTimer(uint32 nInterval);
	//单位是秒
	void StartTimerSec(uint32 nSec);
	//终止定时器
	void StopTimer();
private:
	bool IsEmpty();
	void ClearNode();
	void RemoveNode();
	void AddTail(CTimerNode* const pNode);
private:
	uint32 m_nExpire;
	uint32 m_nTimerId;
	CTimerNode* m_pPrev;
	CTimerNode* m_pNext;
	CServices* m_pService;
};

typedef CTimerNode CTimer;