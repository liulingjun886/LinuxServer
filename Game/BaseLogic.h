#pragma once
#include <assert.h>
#include "../include/types.h"
#include "../GameServer/Room.h"

class CBaseLogic
{
public:
	CBaseLogic(CRoom* pRoom):m_pRoom(pRoom){};
	virtual ~CBaseLogic(){};
public:
	virtual bool InitGameStation(void* pData,USHORT uDataSize) = 0; //解析房间规则
	virtual bool GetGameStation(USHORT uSeatNo) = 0; // 获取游戏状态
	virtual bool HandNetMsg(USHORT uSeatNo, USHORT uSub, void* pData, USHORT uDataSize)=0; //处理网络消息
	virtual bool HandTimeMsg(USHORT uTimeId) = 0; //处理定时器消息
	virtual bool GameBegin() = 0;
	virtual void UserNetCut(USHORT nSeatNo) {}
	virtual void UserLeft(USHORT nSeatNo) {}
	virtual void GameOver() = 0;
protected:
	CRoom* const m_pRoom;
};