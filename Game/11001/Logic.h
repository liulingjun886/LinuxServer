#pragma once
#include "../BaseLogic.h"
#include "../../Defines.h"
#include "../../include/SmartPoint.h"
#include "../../include/TimerNode.h"

class CLogic :
	public CBaseLogic
{	
	CSmartArrayPoint<uint8[5]> m_pCards;
	CSmartArrayPoint<char> m_bShowCards;
	CSmartArrayPoint<char> m_pBank;
	CSmartArrayPoint<int> m_pBets;
	CSmartArrayPoint<int> m_pWin;
	CSmartArrayPoint<uint8> m_pCardType;
	CSmartArrayPoint<uint8> m_pMaxCard;
	
	int m_nBank;
	int m_nBet;
	int m_nGameState;
	int m_nGameStateTime;
	int m_nUserCount;

	CTimer m_timerReady;
	CTimer m_timerBanker;
	CTimer m_timerRestart;
	CTimer m_timerShowBanker;
	CTimer m_timerBet;
	CTimer m_timerDispatchCard;
	CTimer m_timerShowCard;
	CTimer m_timerCompare;
public:
	CLogic(CRoom* pRoom);
	~CLogic();
public:
	bool InitGameStation(void* pData, uint16 nDataSize);
	bool GetGameStation(uint16 uSeatNo);
	bool HandNetMsg(uint16 uSeatNo, uint16 nSub, void* pData, DATASIZE uDataSize);
	bool HandTimeMsg(uint16 uTimeId);
	bool GameBegin();
	void GameOver();
private:
	void RobBank();
	void BeginBet();
	void DispatchCard();
	void CompareCard();
	void UpdateGameState(int nState);

	void UserBet(uint16 nSeatNo,int nBet);
	void UserRobBank(uint16 nSeatNo,int nRob);
	void UserShowCard(uint16 nSeatNo);

	void ConfirmBank();
	void EndReady();
	void EndBanker();
	void EndBet();
	void EndShowCard();
	void EndDispatchCard();
};