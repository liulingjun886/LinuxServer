#pragma once
#include "./Defines.h"
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
	uint32 nDiamond;
	uint64 nJinBi;
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

	void UpdateConnInfo(uint16 nCid,uint16 nCsid);
	void UpdateGameInfo(uint16 nGid,uint16 nGsid, uint16 nGsno);

	void GetConnInfo(uint16& nCid,uint16& nCsid);
	void GetGameInfo(uint16& nGid,uint16& nGsid, uint16& nGsno);

	void SetUserBaseInfo(UID nUserId,char bSex=0,char const *szName=NULL,char const *szHeadUrl=NULL);
	UserBaseInfo* GetUserBaseInfo();
private:
	UserBaseInfo m_tagBaseInfo;
	UserCoinInfo m_tagCoinInfo;
	UserSerInfo m_tagSerInfo;
};
