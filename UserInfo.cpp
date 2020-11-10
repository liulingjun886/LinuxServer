#include "UserInfo.h"
CUserInfo::CUserInfo()
{
	
}

CUserInfo::~CUserInfo()
{
	
}

UID CUserInfo::GetUserId()
{
	return m_tagBaseInfo.nUserId;
}

void CUserInfo::UpdateConnInfo(USHORT nCid,USHORT nCsid)
{
	m_tagSerInfo.nCid = nCid;
	m_tagSerInfo.nCsid = nCsid;
}

void CUserInfo::UpdateGameInfo(USHORT nGid,USHORT nGsid, USHORT nGsno)
{
	m_tagSerInfo.nGid = nGid;
	m_tagSerInfo.nGsid = nGsid;
	m_tagSerInfo.nGsno = nGsno;
}

void CUserInfo::GetConnInfo(USHORT& nCid,USHORT& nCsid)
{
	nCid = m_tagSerInfo.nCid;
	nCsid = m_tagSerInfo.nCsid;
}

void CUserInfo::GetGameInfo(USHORT& nGid,USHORT& nGsid, USHORT& nGsno)
{
	nGid = m_tagSerInfo.nGid;
	nGsid = m_tagSerInfo.nGsid;
	nGsno = m_tagSerInfo.nGsno;
}

void CUserInfo::SetUserBaseInfo(UID nUserId,char bSex, char const *szName, char const *szHeadUrl)
{
	m_tagBaseInfo.nUserId = nUserId;
	m_tagBaseInfo.bSex = bSex;
	if(szName)
		strcpy(m_tagBaseInfo.szName,szName);
	if(szHeadUrl)
		strcpy(m_tagBaseInfo.szHeadUrl,szHeadUrl);
}

UserBaseInfo* CUserInfo::GetUserBaseInfo()
{
	return &m_tagBaseInfo;
}

