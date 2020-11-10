#pragma once
#include "types.h"

enum S_C_PROTOL
{
	S_C_GAMEBEGIN = 1,
	S_C_BEGINROBBANK,
	S_C_CONFIRMBANK,
	S_C_BEGINBET,
	S_C_DISPATCHCARD,
	S_C_USERROBBANK,
	S_C_USERBET,
	S_C_USERSHOWCARD,
	S_C_GAMEFINISH,
	S_C_GAMEOVER
};

enum C_S_PROTOL
{
	C_S_USERROBBANK = 1,
	C_S_USERBET,
	C_S_USERSHOWCARD,
};

enum Timer 
{
	TIMER_READY=1000,
	TIMER_BANKER,
	TIMER_RESTART,
	TIMER_SHOWBANKER,
	TIMER_BET,
	TIMER_DISPATCHCARD,
	TIMER_SHOWCARD,
	TIMER_COMPARE
};

enum GameState
{
	NONE,
	READY,
	BANK,
	BET,
	DISCARD,
	SHOWCARD,
	COMPARE,
	FINISH
};

struct C_S_RobBank 
{
	int nOper;
};

struct C_S_UserBet 
{
	int nMul;
};

struct S_C_DisPatchCard
{
	BYTE nCards[5];
};

struct S_C_UserBet 
{
	USHORT nSeatNo;
	int nMul;
};

struct S_C_UserRobBank 
{
	USHORT nSeatNo;
	int nRob;
};

struct S_C_ConfirmBank 
{
	USHORT nBankSeatNo;
};

struct S_C_UserShowCard 
{
	USHORT nSeatNo;
	USHORT nCardType;
	int nMuls;
	BYTE nCards[5];
};

struct UserWinInfo
{
	USHORT nSeatNo;
	int nWin;
};

struct S_C_UserWininfo 
{
	USHORT nUserCount;
	UserWinInfo userWinInfo[1];
};