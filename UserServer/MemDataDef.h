#pragma once

namespace Mem
{
	enum TYPE_MEMDATABASE_REQ
	{
		USER_LOGIN_REQ = 1,
		USER_LOGOUT_REQ,
		USER_JOIN_GAME_REQ,
		USER_QUIT_GAME_REQ,
	};

	enum TYPE_MEMDATABASE_RET
	{
		USER_LOGIN_RET = 1,
		USER_JOIN_GAME_RET,
	};
	
	struct UserLoginMemReq
	{
		int nUserId;
		uint16 nSerNo;
		uint16 nSockNo;

		UserLoginMemReq()
		{
			memset(this,0,sizeof(UserLoginMemReq));
		}
	};

	struct UserLoginMemRet
	{
		uint16 nCid;
		uint16 nGid;
		SERVICEINDEX nCSid;
		SERVICEINDEX nGSid;
		uint16 nGsno;

		UserLoginMemRet()
		{
			memset(this,0,sizeof(UserLoginMemReq));
		}
	};

	struct UserLogOutMemReq
	{
		int nUserId;
		uint16 nSerNo;
		uint16 nSockNo;

		UserLogOutMemReq()
		{
			memset(this,0,sizeof(UserLogOutMemReq));
		}
	};

	struct UserJoinGameReq
	{
		int nUserId;
		int nSeatNo;
		uint16 nGid;
		SERVICEINDEX nGsid;
		
		UserJoinGameReq()
		{
			memset(this,0,sizeof(UserJoinGameReq));
			nSeatNo = -1;
		}
	};

	struct UserJoinGameRet
	{
		int nSeatNo;
		uint16 nCid;
		SERVICEINDEX nCsid;
		UserJoinGameRet()
		{
			memset(this,0,sizeof(UserJoinGameRet));
		}
	};

	struct UserQuitGameReq
	{
		int nUserId;
		UserQuitGameReq()
		{
			memset(this,0,sizeof(UserQuitGameReq));
		}
	};
}
