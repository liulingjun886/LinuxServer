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
		USHORT nSerNo;
		USHORT nSockNo;

		UserLoginMemReq()
		{
			memset(this,0,sizeof(UserLoginMemReq));
		}
	};

	struct UserLoginMemRet
	{
		USHORT nCid;
		USHORT nGid;
		SERVICEINDEX nCSid;
		SERVICEINDEX nGSid;
		USHORT nGsno;

		UserLoginMemRet()
		{
			memset(this,0,sizeof(UserLoginMemReq));
		}
	};

	struct UserLogOutMemReq
	{
		int nUserId;
		USHORT nSerNo;
		USHORT nSockNo;

		UserLogOutMemReq()
		{
			memset(this,0,sizeof(UserLogOutMemReq));
		}
	};

	struct UserJoinGameReq
	{
		int nUserId;
		int nSeatNo;
		USHORT nGid;
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
		USHORT nCid;
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
