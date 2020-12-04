#include "Logic.h"
#include "stdio.h"
#include "UpGradeLogic.h"
#include "UpGradeMessage.h"
#include <time.h>
#include <vector>
#include <stdlib.h>


CLogic::CLogic(CRoom* pRoom) :CBaseLogic(pRoom)
{
	m_timerReady.InitTimerObj(m_pRoom, TIMER_READY);
	m_timerBanker.InitTimerObj(m_pRoom, TIMER_BANKER);
	m_timerRestart.InitTimerObj(m_pRoom, TIMER_RESTART);
	m_timerShowBanker.InitTimerObj(m_pRoom, TIMER_SHOWBANKER);
	m_timerBet.InitTimerObj(m_pRoom, TIMER_BET);
	m_timerDispatchCard.InitTimerObj(m_pRoom, TIMER_DISPATCHCARD);
	m_timerShowCard.InitTimerObj(m_pRoom, TIMER_SHOWCARD);
	m_timerCompare.InitTimerObj(m_pRoom, TIMER_COMPARE);
}

CLogic::~CLogic()
{
	
}

bool CLogic::InitGameStation(void* pData, uint16 nDataSize)
{
	m_nUserCount = m_pRoom->GetUserCount();
	m_pCards.Init(m_nUserCount);
	m_pBets.Init(m_nUserCount);
	m_pWin.Init(m_nUserCount);
	m_pCardType.Init(m_nUserCount);
	m_pMaxCard.Init(m_nUserCount);
	m_pBank.Init(m_nUserCount);
	m_bShowCards.Init(m_nUserCount);
	return true;
}

bool CLogic::GetGameStation(uint16 nSeatNo)
{
	return true;
}

bool CLogic::HandNetMsg(uint16 nSeatNo, uint16 nSub, CInputPacket& inPacket)
{
	switch (nSub)
	{
		case C_S_USERROBBANK:
		{
			C_S_RobBank* pBank = (C_S_RobBank*)inPacket.ReadBinary(sizeof(C_S_RobBank));
			UserRobBank(nSeatNo, pBank->nOper);
			break;
		}
		case C_S_USERBET:
		{
			C_S_UserBet *pBet = (C_S_UserBet*)inPacket.ReadBinary(sizeof(C_S_UserBet));
			UserBet(nSeatNo, pBet->nMul);
			break;
		}
		case C_S_USERSHOWCARD:
		{
			UserShowCard(nSeatNo);
			break;
		}
		default:
			break;
	}
	return true;
}

void CLogic::EndReady()
{
	RobBank();
}

void CLogic::EndBanker()
{
	for(int i = 0; i < m_nUserCount; i++)
	{
		if(!m_pRoom->GetUsers(i))
			continue;
		if(m_pBank[i] == 0)
			UserRobBank(i,1);
	}
}

void CLogic::EndBet()
{
	for(int i = 0; i < m_nUserCount; i++)
	{
		if(!m_pRoom->GetUsers(i))
			continue;
		if(i == m_nBank)
			continue;
		if(m_pBets[i] == 0)
			UserBet(i,1);
	}
}

void CLogic::EndDispatchCard()
{
	UpdateGameState(SHOWCARD);
	COutputPacket out;
	out.Begin(MAIN_MSG_GAME,S_C_USERSHOWCARD);
	out.End();
	m_pRoom->SendDataToAll(out);
	m_timerShowCard.StartTimerSec(10);
}


void CLogic::EndShowCard()
{
	for(int i = 0; i < m_nUserCount; i++)
	{
		if(!m_pRoom->GetUsers(i))
			continue;

		if(m_bShowCards[i] == 0)
			UserShowCard(i);
	}
}

bool CLogic::HandTimeMsg(uint16 nTimeId)
{

	switch (nTimeId)
	{
		case TIMER_READY:
		{
			RobBank();
			break;
		}
		case TIMER_BANKER:
		{
			EndBanker();
			break;
		}
		case TIMER_SHOWBANKER:
		{
			BeginBet();
			break;
		}
		case TIMER_BET:
		{
			EndBet();
			break;
		}
		case TIMER_DISPATCHCARD:
		{
			EndDispatchCard();
			break;
		}
		case TIMER_SHOWCARD:
		{
			EndShowCard();
			break;
		}
		case TIMER_COMPARE:
		{
			CompareCard();
			break;
		}
		case TIMER_RESTART:
		{
			GameBegin();
			break;
		}
		default:
			break;
	}
	return true;
}

bool CLogic::GameBegin()
{
	m_pCards.Reset(0);
	m_pBets.Reset(0);
	m_pWin.Reset(0);
	m_pCardType.Reset(0);
	m_pMaxCard.Reset(0);
	m_pBank.Reset(0);
	m_bShowCards.Reset(0);
	UpdateGameState(READY);
	COutputPacket out;
	out.Begin(MAIN_MSG_GAME, S_C_GAMEBEGIN);
	out.End();
	m_pRoom->SendDataToAll(out);
	m_timerReady.StartTimerSec(3);
	return true;
}

void CLogic::GameOver()
{
	COutputPacket out;
	out.Begin(MAIN_MSG_GAME, S_C_GAMEOVER);
	out.End();
	m_pRoom->SendDataToAll(out);
}


void CLogic::RobBank()
{
	UpdateGameState(BANK);
	COutputPacket out;
	out.Begin(MAIN_MSG_GAME, S_C_BEGINROBBANK);
	out.End();
	m_pRoom->SendDataToAll(out);
	m_timerBanker.StartTimerSec(10);
}

void CLogic::BeginBet()
{
	UpdateGameState(BET);
	COutputPacket out;
	out.Begin(MAIN_MSG_GAME, S_C_BEGINBET);
	out.End();
	m_pRoom->SendDataToAll(out);
	m_timerBet.StartTimerSec(20);
}

void CLogic::DispatchCard()
{
	UpdateGameState(DISCARD);
	m_timerBet.StopTimer();
	
	uint8 nCardArray[54] =
	{
		0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //方块 2 - A
		0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //梅花 2 - A
		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //红桃 2 - A
		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //黑桃 2 - A
		0x4E, 0x4F //小鬼，大鬼
	};

	CUpGradeGameLogic::RandCards(nCardArray, 52);
	int nIndex = 0;
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < m_nUserCount; j++)
		{
			if(!m_pRoom->GetUsers(j))
				continue;

			m_pCards[j][i] = nCardArray[nIndex++];
		}
	}

	
	//TODO:发送牌数据
	S_C_DisPatchCard nCards;
	COutputPacket out;
	for(int i = 0; i < m_nUserCount; i++)
	{
		if(!m_pRoom->GetUsers(i))
			continue;
		memcpy(&nCards,m_pCards[i],sizeof(nCards));
		out.Begin(MAIN_MSG_GAME, S_C_DISPATCHCARD);
		out.WriteBinary(&nCards, sizeof(nCards));
		out.End();
		m_pRoom->SendDataToUser(i, out);
	}
	m_timerDispatchCard.StartTimerSec(3);
}

void CLogic::CompareCard()
{
	UpdateGameState(COMPARE);
	for(int i = 0; i < m_nUserCount; i++)
	{
		if(i == m_nBank)
			continue;
		if(!m_pRoom->GetUsers(i))
			continue;

		if(CUpGradeGameLogic::CompareCard(m_pCardType[m_nBank], m_pMaxCard[m_nBank], m_pCardType[i], m_pMaxCard[i]))
		{
			int nMul = CUpGradeGameLogic::GetMulByCardType(m_pCardType[m_nBank]);
			m_pWin[i] -= m_pWin[i]*nMul;
			m_pWin[m_nBank] += m_pWin[i];
		}
		else
		{
			int nMul = CUpGradeGameLogic::GetMulByCardType(m_pCardType[i]);
			m_pWin[i] += m_pWin[i]*nMul;
			m_pWin[m_nBank] -= m_pWin[i];
		}
	}

	
	
	int nUserCount = 0;
	char buff[MAX_MSG_SIZE] = {0};
	S_C_UserWininfo* pWinInfo = (S_C_UserWininfo*)buff;
	for(int i = 0; i < m_nUserCount; i++)
	{
		if(!m_pRoom->GetUsers(i))
			continue;

		pWinInfo->userWinInfo[nUserCount].nSeatNo = i;
		pWinInfo->userWinInfo[nUserCount].nWin = m_pWin[i];
		++nUserCount;
	}
	pWinInfo->nUserCount = nUserCount;

	COutputPacket out;
	out.Begin(MAIN_MSG_GAME, S_C_GAMEFINISH);
	out.WriteBinary(buff, (uint32)(sizeof(UserWinInfo)*nUserCount+sizeof(pWinInfo->nUserCount)));
	out.End();
	m_pRoom->SendDataToAll(out);
	
	m_pRoom->GameFinish();
}

void CLogic::UpdateGameState(int nState)
{
	m_nGameState = nState;
	m_nGameStateTime = time(NULL);
}


void CLogic::UserBet(uint16 nSeatNo,int nBet)
{
	if(m_pBets[nSeatNo] > 0)
		return;
	m_pBets[nSeatNo] = nBet;

	S_C_UserBet bet;
	bet.nSeatNo = nSeatNo;
	bet.nMul = nBet;
	
	COutputPacket out;
	out.Begin(MAIN_MSG_GAME, S_C_USERBET);
	out.WriteBinary(&bet, (uint32)sizeof(S_C_UserBet));
	out.End();
	m_pRoom->SendDataToAll(out);
	
	for(int i = 0; i < m_nUserCount; i++)
	{
		if(!m_pRoom->GetUsers(i))
			continue;

		if(i == m_nBank)
			continue;

		if(m_pBets[i] == 0)
			return;
	}
	DispatchCard();
}

void CLogic::UserRobBank(uint16 nSeatNo,int nRob)
{
	if(m_nGameState != BANK)
		return;
	
	if(m_pBank[nSeatNo] > 0)
		return;

	m_pBank[nSeatNo] = nRob;

	S_C_UserRobBank robBank;
	robBank.nSeatNo = nSeatNo;
	robBank.nRob = nRob;

	COutputPacket out;
	out.Begin(MAIN_MSG_GAME, S_C_USERROBBANK);
	out.WriteBinary(&robBank, (uint32)sizeof(S_C_UserRobBank));
	out.End();
	m_pRoom->SendDataToAll(out);

	for(int i = 0; i < m_nUserCount; i++)
	{
		if(!m_pRoom->GetUsers(i))
			continue;

		if(m_pBank[nSeatNo] == 0)
			return;
	}
	
	ConfirmBank();
}

void CLogic::UserShowCard(uint16 nSeatNo)
{
	if(m_nGameState != SHOWCARD)
		return;
	
	if(m_bShowCards[nSeatNo] == 1)
		return;

	m_bShowCards[nSeatNo] = 1;

	m_pCardType[nSeatNo] = CUpGradeGameLogic::GetCardType(m_pCards[nSeatNo], sizeof(m_pCards[nSeatNo]), m_pMaxCard[nSeatNo]);

	S_C_UserShowCard userShowCard;
	userShowCard.nSeatNo = nSeatNo;
	userShowCard.nCardType = m_pCardType[nSeatNo];
	userShowCard.nMuls = CUpGradeGameLogic::GetMulByCardType(userShowCard.nCardType);
	memcpy(&userShowCard.nCards,m_pCards[nSeatNo],sizeof(userShowCard.nCards));

	COutputPacket out;
	out.Begin(MAIN_MSG_GAME,S_C_USERSHOWCARD);
	out.WriteBinary(&userShowCard, (uint32)sizeof(S_C_UserShowCard));
	out.End();
	m_pRoom->SendDataToAll(out);

	for(int i = 0; i < m_nUserCount; i++)
	{
		if(!m_pRoom->GetUsers(i))
			continue;
		
		if(m_bShowCards[nSeatNo] == 0)
			return;
	}
	
	m_timerShowCard.StopTimer();
	m_timerCompare.StartTimerSec(1);
}

void CLogic::ConfirmBank()
{
	m_timerBanker.StopTimer();
	std::vector<int> vecBank;
	for(int i = 0; i < m_nUserCount; i++)
	{
		if(m_pBank[i] == 1)
		{
			vecBank.push_back(i);
		}
	}
	if(vecBank.size() == 0)
	{
		//TODO:发送游戏重新开始消息
		m_timerRestart.StartTimerSec(1);
		return;
	}
	m_nBank = vecBank[rand()%vecBank.size()];

	S_C_ConfirmBank confirmBank;
	confirmBank.nBankSeatNo = m_nBank;

	COutputPacket out;
	out.Begin(MAIN_MSG_GAME, S_C_CONFIRMBANK);
	out.WriteBinary(&confirmBank, (uint32)sizeof(S_C_ConfirmBank));
	out.End();
	m_pRoom->SendDataToAll(out);
	
	m_timerShowBanker.StartTimerSec(1);
	
}

extern "C" CBaseLogic* CreateLogic(CRoom* pRoom)
{
	return new CLogic(pRoom);
}
