#pragma once

#include "../../Defines.h"

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
	static int GetCardType(uint8 nCards[],int nCardCount,uint8& nMaxCard);
	static bool CompareCard(int nCardType1,uint8 nMaxCard1,int nCardType2,uint8 nMaxCard2);
	static void RandCards(uint8 nCards[],int nCardCount);
	static int GetMulByCardType(uint8 nCardType);
private:
	static int GetCardNum(uint8 nCard);
	static int GetCardKind(uint8 nCard);
	static int SortCards(uint8 nCards[],int nCardCount);
	static int GetPoint(uint8 nCard);
	static void SwapCard(uint8& nCard1,uint8& nCard2);
	static bool IsGoldBull(uint8	nCards[]);
	static bool IsSilverBull(uint8	   nCards[]);
	static bool IsBombBull(uint8	nCards[]);
	static bool IsFiveSmall(uint8	  nCards[]);
	
};


