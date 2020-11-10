#pragma once
namespace DataBase
{
	enum TYPE_DATABASE_REQ
	{
		USER_LOGIN_REQ = 1
	};

	enum TYPE_DATABASE_RET
	{
		USER_LOGIN_RET = 1
	};

	struct UserLoginReq
	{
		int nUserId;
		char szPass[16];
		UserLoginReq()
		{
			memset(this, 0, sizeof(UserLoginReq));
		}
	};

	struct UserLoginRet
	{
		int ret;
		int nUserId;
		char nSex;
		char szName[64];
		char szHeadUrl[128];
		UserLoginRet()
		{
			memset(this, 0, sizeof(UserLoginRet));
		}
	};
}