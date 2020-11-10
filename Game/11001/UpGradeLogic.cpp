#include "UpGradeLogic.h"
#include <stdlib.h>

CUpGradeGameLogic::CUpGradeGameLogic()
{
}

CUpGradeGameLogic::~CUpGradeGameLogic()
{
}

int CUpGradeGameLogic::GetCardNum(BYTE nCard)
{
	return (nCard&UG_VALUE_MASK)+1;
}

int CUpGradeGameLogic::GetCardKind(BYTE nCard)
{
	return nCard&UG_HUA_MASK;
}

int CUpGradeGameLogic::SortCards(BYTE nCards[],int nCardCount)
{
	for(int i = 0; i < nCardCount-1; i++)
	{
		for(int j = i+1; j < nCardCount; j++)
		{
			if(GetPoint(nCards[i]) < GetPoint(nCards[j]))
			{
				SwapCard(nCards[i], nCards[j]);
				continue;
			}
			
			if(GetCardNum(nCards[i]) == GetCardNum(nCards[j]))
			{
				if(GetCardKind(nCards[i]) > GetCardKind(nCards[j]))
				{
					SwapCard(nCards[i], nCards[j]);
				}
			}
			else
			{
				if(GetCardNum(nCards[i]) > GetCardNum(nCards[j]))
				{
					SwapCard(nCards[i], nCards[j]);
				}
			}
		}
	}
	return 0;
}

int CUpGradeGameLogic::GetCardType(BYTE nCards[],int nCardCount,BYTE& nMaxCard)
{
	if(nCardCount != 5)
		return 0;
	SortCards(nCards,nCardCount);
	nMaxCard = nCards[4];

	if(IsFiveSmall(nCards))
		return UG_FIVE_SMALL;

	if(IsBombBull(nCards))
		return UG_BULL_BOMB;

	if(IsGoldBull(nCards))
		return UG_BULL_GOLD;

	if(IsSilverBull(nCards))
		return UG_BULL_SILVER;
	
	int nSum = GetPoint(nCards[0])+GetPoint(nCards[1])+GetPoint(nCards[2])+GetPoint(nCards[3])+GetPoint(nCards[4]);
	int nRest = nSum % 10;

	for(int i = 0; i < nCardCount - 1; i++)
	{
		for(int j = i+1; j < nCardCount; j++)
		{
			if((GetPoint(nCards[i])+GetPoint(nCards[j]))%10 == nRest)
			{
				SwapCard(nCards[j],nCards[4]);
				SwapCard(nCards[i],nCards[3]);
				if(nRest == 0)
					return UG_BULL_BULL;
				return nRest;
			}
		}
	}
	return UG_NO_POINT;
}

int CUpGradeGameLogic::GetPoint(BYTE nCard)
{
	switch (nCard)
	{
		case 0x0D:
			return 1;
		case 0x0A:
		case 0x0B:
		case 0x0C:
			return 10;
		default:
			return GetCardNum(nCard);
	}
}

void CUpGradeGameLogic::SwapCard(BYTE& nCard1,BYTE& nCard2)
{
	if(nCard1 == nCard2)
		return;

	BYTE bTemp = nCard1;
	nCard1 = nCard2;
	nCard2 = bTemp;
}

bool CUpGradeGameLogic::IsGoldBull(BYTE    nCards[])
{
	if(GetCardNum(nCards[1]) >   0x09 && GetCardNum(nCards[4]) < 0x0D)
		return true;
	return false;
}

bool CUpGradeGameLogic::IsSilverBull(BYTE     nCards[])
{
	if(GetCardNum(nCards[1]) >   0x08 && GetCardNum(nCards[4]) < 0x0D)
		return true;
	return false;
}

bool CUpGradeGameLogic::IsBombBull(BYTE    nCards[])
{
	if(GetCardNum(nCards[0]) == GetCardNum(nCards[3]) || GetCardNum(nCards[1]) == GetCardNum(nCards[4]))
		return true;
	return false;
}

bool CUpGradeGameLogic::IsFiveSmall(BYTE     nCards[])
{
	if(GetPoint(nCards[4]) > 4)
		return false;
	if(GetPoint(nCards[0])+GetPoint(nCards[1])+GetPoint(nCards[2])+GetPoint(nCards[3])+GetPoint(nCards[4]) > 9)
		return false;
	return true;
}

bool CUpGradeGameLogic::CompareCard(int nCardType1,BYTE nMaxCard1,int nCardType2,BYTE nMaxCard2)
{
	if(nCardType1 == nCardType2)
	{
		if(GetCardNum(nMaxCard1) == GetCardNum(nMaxCard2))
			return GetCardKind(nMaxCard1) > GetCardKind(nMaxCard1);
		return GetCardNum(nMaxCard1) > GetCardNum(nMaxCard2);
	}
	return nCardType1 > nCardType2;
}

void CUpGradeGameLogic::RandCards(BYTE nCards[],int nCardCount)
{
	for(int i = 0,j = nCardCount-1; i < nCardCount; ++i,--j)
	{
		int nIndex = rand()%nCardCount;
		SwapCard(nCards[i],nCards[nIndex]);
		nIndex = rand()%nCardCount;
		SwapCard(nCards[j],nCards[nIndex]);
	}
}

int CUpGradeGameLogic::GetMulByCardType(BYTE nCardType)
{
	switch (nCardType)
	{
		case UG_BULL_SEVEN:
		case UG_BULL_EIGHT:
		case UG_BULL_NINE:
			return 2;
		case UG_BULL_BULL:
			return 3;
		case UG_BULL_SILVER:
			return 4;
		case UG_BULL_GOLD:
			return 5;
		case UG_BULL_BOMB:
			return 6;
		case UG_FIVE_SMALL:
			return 7;
		default:
			return 1;
	}
}

