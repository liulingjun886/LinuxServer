#pragma once
#include "../BaseLogic.h"
#include "SmartPoint.h"
#include "TimerNode.h"

class CLogic :
	public CBaseLogic
{	
	CSmartArrayPoint<BYTE[5]> m_pCards;
	CSmartArrayPoint<char> m_bShowCards;
	CSmartArrayPoint<char> m_pBank;
	CSmartArrayPoint<int> m_pBets;
	CSmartArrayPoint<int> m_pWin;
	CSmartArrayPoint<BYTE> m_pCardType;
	CSmartArrayPoint<BYTE> m_pMaxCard;
	
	int m_nBank;
	int m_nBet;
	int m_nGameState;
	int m_nGameStateTime;
	int m_nUserCount;

	CTimerNode m_timerReady;
	CTimerNode m_timerBanker;
	CTimerNode m_timerRestart;
	CTimerNode m_timerShowBanker;
	CTimerNode m_timerBet;
	CTimerNode m_timerDispatchCard;
	CTimerNode m_timerShowCard;
	CTimerNode m_timerCompare;
public:
	CLogic(CRoom* pRoom);
	~CLogic();
public:
	bool InitGameStation(void* pData, USHORT nDataSize);
	bool GetGameStation(USHORT uSeatNo);
	bool HandNetMsg(USHORT uSeatNo, USHORT nSub, void* pData, USHORT uDataSize);
	bool HandTimeMsg(USHORT uTimeId);
	bool GameBegin();
	void GameOver();
private:
	void RobBank();
	void BeginBet();
	void DispatchCard();
	void CompareCard();
	void UpdateGameState(int nState);

	void UserBet(USHORT nSeatNo,int nBet);
	void UserRobBank(USHORT nSeatNo,int nRob);
	void UserShowCard(USHORT nSeatNo);

	void ConfirmBank();
	void EndReady();
	void EndBanker();
	void EndBet();
	void EndShowCard();
	void EndDispatchCard();
};