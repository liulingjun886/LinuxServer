#pragma once
#include <assert.h>
#include "../Defines.h"
#include "../GameServer/Room.h"

class CBaseLogic
{
public:
	CBaseLogic(CRoom* pRoom):m_pRoom(pRoom){};
	virtual ~CBaseLogic(){};
public:
	virtual bool InitGameStation(void* pData,uint16 uDataSize) = 0; //解析房间规则
	virtual bool GetGameStation(uint16 uSeatNo) = 0; // 获取游戏状态
	virtual bool HandNetMsg(uint16 uSeatNo, uint16 uSub, void* pData, DATASIZE uDataSize)=0; //处理网络消息
	virtual bool HandTimeMsg(uint16 uTimeId) = 0; //处理定时器消息
	virtual bool GameBegin() = 0;
	virtual void UserNetCut(uint16 nSeatNo) {}
	virtual void UserLeft(uint16 nSeatNo) {}
	virtual void GameOver() = 0;
protected:
	CRoom* const m_pRoom;
};