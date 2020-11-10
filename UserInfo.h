#pragma once
#include "types.h"
struct UserBaseInfo
{
	UID nUserId;
	char szName[32];
	char bSex;
	char szHeadUrl[128];

	UserBaseInfo()
	{
		memset(this,0,sizeof(UserBaseInfo));
	}
};

struct UserCoinInfo
{
	UINT nDiamond;
	UINT64 nJinBi;
	UserCoinInfo()
	{
		memset(this,0,sizeof(UserCoinInfo));
	}
};

struct UserSerInfo
{
	int nCid;
	int nCsid;
	int nGid;
	int nGsid;
	int nGsno;

	UserSerInfo()
	{
		memset(this,0,sizeof(UserSerInfo));
	}
};


class CUserInfo
{
	public:
		CUserInfo();
		~CUserInfo();
	public:
		UID GetUserId();

		void UpdateConnInfo(USHORT nCid,USHORT nCsid);
		void UpdateGameInfo(USHORT nGid,USHORT nGsid, USHORT nGsno);

		void GetConnInfo(USHORT& nCid,USHORT& nCsid);
		void GetGameInfo(USHORT& nGid,USHORT& nGsid, USHORT& nGsno);

		void SetUserBaseInfo(UID nUserId,char bSex=0,char const *szName=NULL,char const *szHeadUrl=NULL);
		UserBaseInfo* GetUserBaseInfo();
	private:
		UserBaseInfo m_tagBaseInfo;
		UserCoinInfo m_tagCoinInfo;
		UserSerInfo m_tagSerInfo;
};
