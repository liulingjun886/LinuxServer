#include "CenSerSink.h"

extern CServer* g_pSer;

enum TIMER_ID
{
	TIME_TEST_LINK = 1,
};

CCenSerSink::CCenSerSink(CServices* pServices):CNetHandSink(pServices)
{
	m_timerTestLink.InitTimerObj(pServices, TIME_TEST_LINK);
	m_pServer = dynamic_cast<CCenterServer*>(g_pSer);
	if(NULL == m_pServer)
		exit(0);
}

CCenSerSink::~CCenSerSink()
{
	
}

void CCenSerSink::Connect()
{
	m_nTestNum = 0;
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CENTER, SUB_MSG_CONN_SUCSS);
}

bool CCenSerSink::DisConnect()
{
	std::map<USHORT,USHORT*>::iterator it = m_pServer->m_mapLinkInfo.find(m_pNet->GetServiceIndex());
	if(it == m_pServer->m_mapLinkInfo.end())
		return false;

	*(it->second) = 0;
	m_pServer->m_mapLinkInfo.erase(it);

	std::map<USHORT,GameInfo>::iterator it_game = m_pServer->m_mapGameInfo.find(m_pNet->GetServiceIndex());
	if(it_game != m_pServer->m_mapGameInfo.end())
	{
		m_pServer->m_mapGameInfo.erase(it_game);
	}
	return false;
}


bool CCenSerSink::HandNetData(USHORT nSrcIndex, USHORT nMain, USHORT nSub, void* pData, UINT nDataSize)
{
	switch(nMain)
	{
		case MAIN_MSG_NET:
		{
			return HandMainNetMsg(nSub, pData, nDataSize);
		}
		default:
		{
			break;
		}
	}
	return false;
}

bool CCenSerSink::HandTimeMsg(USHORT nTimeID)
{
	switch(nTimeID)
	{
		case TIME_TEST_LINK:
		{
			++m_nTestNum;
			if(m_nTestNum > 1)
				return false;
			
			m_timerTestLink.StartTimerSec(300);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandMainNetMsg(USHORT nSrcIndex, USHORT nSub, void* pData, UINT nDataSize)
{
	switch(nSub)
	{
		case SUB_MSG_TEST:
		{
			m_nTestNum = 0;
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_NET, nSub);
			return true;
		}
		case SUB_MSG_REG_USERSER:
		{
			RegConnSer* pSer = (RegConnSer*)pData;
			if(0 != m_pServer->s_szUserSer[pSer->nSerNo])
				return false;
			m_pServer->s_szConnSer[pSer->nSerNo] = m_pNet->GetServiceIndex();
			m_pServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(m_pServer->s_szConnSer[pSer->nSerNo]);
			return true;
		}
		case SUB_MSG_REG_DATASER:
		{
			RegConnSer* pSer = (RegConnSer*)pData;
			if(0 != m_pServer->s_szDataSer[pSer->nSerNo])
				return false;

			m_pServer->s_szDataSer[pSer->nSerNo] = m_pNet->GetServiceIndex();
			m_pServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(m_pServer->s_szDataSer[pSer->nSerNo]);
			return true;
		}
		case SUB_MSG_REG_GAMESER:
		{
			RegGameSer* pGameSer = (RegGameSer*)pData;
			if(0 != m_pServer->s_szGameSer[pGameSer->nSerNo])
				return false;

			m_pServer->s_szGameSer[pGameSer->nSerNo] = m_pNet->GetServiceIndex();
			m_pServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(m_pServer->s_szGameSer[pGameSer->nSerNo]);

			GameInfo game;
			game.szIp = pGameSer->szIp;
			game.nPort = pGameSer->nPort;
			game.nGameID = pGameSer->nGameID;

			m_pServer->m_mapGameInfo[m_pNet->GetServiceIndex()] = game;

			BroadCastGameSerInfo(game);
			return true;
		}
		case SUB_MSG_REG_CONNECTSER:
		{
			RegConnSer* pSer = (RegConnSer*)pData;
			if(0 != m_pServer->s_szConnSer[pSer->nSerNo])
				return false;

			m_pServer->s_szConnSer[pSer->nSerNo] = m_pNet->GetServiceIndex();
			m_pServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(m_pServer->s_szConnSer[pSer->nSerNo]);

			SendAllGameSerInfo();
		}
		default:
		{
			return false;
		}
	}
}

void CCenSerSink::BroadCastGameSerInfo(const GameInfo& gameInfo)
{
	UINT nSize = sizeof(GameInfo) + sizeof(USHORT);
	char* pData = new char[sizeof(GameInfo) + sizeof(USHORT)];
	*(USHORT*)pData = 1;
	GameInfo* pGame = (GameInfo*)(pData+2);
	memcpy(pGame->szIp, gameInfo.szIp.c_str(),strlen(gameInfo.szIp.c_str()));
	pGame->nPort = gameInfo.nPort;
	pGame->nGameID = gameInfo.nGameID;

	for(int i = 0; i < 500; i++)
	{
		if(0 == m_pServer->s_szConnSer[i])
			continue;
		CNetSinkObj::SendData(m_pNet, m_pServer->s_szConnSer[i], MAIN_MSG_CENTER, SUB_MSG_NEWGAMESER, pData, nSize);
	}
}


void CCenSerSink::SendAllGameSerInfo()
{
	UINT nSize = sizeof(GameInfo) * m_pServer->m_mapGameInfo.size() + sizeof(USHORT);
	char* pData = new char[nSize];
	*(USHORT*)pData = m_pServer->m_mapGameInfo.size();
	GameInfo* pGame = (GameInfo*)(pData+2);
	std::map<USHORT,GameInfo>::iterator it_game = m_pServer->m_mapGameInfo.begin();
	for(; it_game != m_pServer->m_mapGameInfo.end(); ++it_game)
	{
		memcpy(pGame->szIp, it_game->second.szIp.c_str(),strlen(it_game->second.szIp.c_str()));
		pGame->nPort = it_game->second.nPort;
		pGame->nGameID = it_game->second.nGameID;
		pGame += 1;
	}
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CENTER, SUB_MSG_NEWGAMESER, pData, nSize);
}


