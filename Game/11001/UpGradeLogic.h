#pragma once

#include "types.h"

#define	UG_HUA_MASK		0xF0
#define	UG_VALUE_MASK	0x0F


enum UG_CARD_TYPE
{
	UG_NO_POINT,
	UG_BULL_ONE,
	UG_BULL_TWO,
	UG_BULL_THREE,
	UG_BULL_FOUR,
	UG_BULL_FIVE,
	UG_BULL_SIX,
	UG_BULL_SEVEN,
	UG_BULL_EIGHT,
	UG_BULL_NINE,
	UG_BULL_BULL,
	UG_BULL_SILVER,
	UG_BULL_GOLD,
	UG_BULL_BOMB,
	UG_FIVE_SMALL
};

class CUpGradeGameLogic
{
public:
	CUpGradeGameLogic();
	~CUpGradeGameLogic();
public:
	static int GetCardType(BYTE nCards[],int nCardCount,BYTE& nMaxCard);
	static bool CompareCard(int nCardType1,BYTE nMaxCard1,int nCardType2,BYTE nMaxCard2);
	static void RandCards(BYTE nCards[],int nCardCount);
	static int GetMulByCardType(BYTE nCardType);
private:
	static int GetCardNum(BYTE nCard);
	static int GetCardKind(BYTE nCard);
	static int SortCards(BYTE nCards[],int nCardCount);
	static int GetPoint(BYTE nCard);
	static void SwapCard(BYTE& nCard1,BYTE& nCard2);
	static bool IsGoldBull(BYTE	nCards[]);
	static bool IsSilverBull(BYTE	   nCards[]);
	static bool IsBombBull(BYTE	nCards[]);
	static bool IsFiveSmall(BYTE	  nCards[]);
	
};


