#include "CenSerSink.h"

extern CCenterServer* g_pCenterServer;

enum TIMER_ID
{
	TIME_TEST_LINK = 1,
};

CCenSerSink::CCenSerSink(CServices* pServices):CNetHandSink(pServices)
{
	m_timerTestLink.InitTimerObj(pServices, TIME_TEST_LINK);
}

CCenSerSink::~CCenSerSink()
{
	
}

void CCenSerSink::Connect()
{
	m_nTestNum = 0;
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CENTERSER, CT_SUB_MSG_CONN_SUCSS);
}

bool CCenSerSink::DisConnect()
{
	std::map<USHORT,USHORT*>::iterator it = g_pCenterServer->m_mapLinkInfo.find(m_pNet->GetServiceIndex());
	if(it == g_pCenterServer->m_mapLinkInfo.end())
		return false;

	*(it->second) = 0;
	g_pCenterServer->m_mapLinkInfo.erase(it);

	std::map<USHORT,GameInfo>::iterator it_game = g_pCenterServer->m_mapGameInfo.find(m_pNet->GetServiceIndex());
	if(it_game != g_pCenterServer->m_mapGameInfo.end())
	{
		g_pCenterServer->m_mapGameInfo.erase(it_game);
	}
	return false;
}


bool CCenSerSink::HandNetData(USHORT nSrcIndex, USHORT nMain, USHORT nSub, void* pData, UINT nDataSize)
{
	switch(nMain)
	{
		case MAIN_MSG_NET:
		{
			return HandMainNetMsg(nSrcIndex, nSub, pData, nDataSize);
		}
		case MAIN_MSG_CONNSER:
		{
			return HandMainMsgFromConnSrv(nSrcIndex, nSub, pData, nDataSize);
		}
		case MAIN_MSG_GAMESER:
		{
			return HandMainMsgFromGameSrv(nSrcIndex, nSub, pData, nDataSize);
		}
		case MAIN_MSG_USERSER:
		{
			return HandMainMsgFromUserSrv(nSrcIndex, nSub, pData, nDataSize);
		}
		case MAIN_MSG_DATASER:
		{
			return HandMainMsgFromDataSrv(nSrcIndex, nSub, pData, nDataSize);
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
		case NT_SUB_MSG_TEST:
		{
			m_nTestNum = 0;
			CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_NET, nSub);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandMainMsgFromUserSrv(USHORT nSrcIndex, USHORT nSub, void* pData, UINT nDataSize)
{
	switch(nSub)
	{
		case US_SUB_MSG_CONN_SUCSS:
		{
			RegConnSer* pSer = (RegConnSer*)pData;
			if(0 != g_pCenterServer->s_szUserSer[pSer->nSerNo])
				return false;
			g_pCenterServer->s_szConnSer[pSer->nSerNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szConnSer[pSer->nSerNo]);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandMainMsgFromDataSrv(USHORT nSrcIndex, USHORT nSub, void* pData, UINT nDataSize)
{
	switch(nSub)
	{
		case US_SUB_MSG_CONN_SUCSS:
		{
			RegConnSer* pSer = (RegConnSer*)pData;
			if(0 != g_pCenterServer->s_szDataSer[pSer->nSerNo])
				return false;

			g_pCenterServer->s_szDataSer[pSer->nSerNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szDataSer[pSer->nSerNo]);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandMainMsgFromGameSrv(USHORT nSrcIndex, USHORT nSub, void* pData, UINT nDataSize)
{
	switch(nSub)
	{
		case US_SUB_MSG_CONN_SUCSS:
		{
			RegGameSer* pGameSer = (RegGameSer*)pData;
			if(0 != g_pCenterServer->s_szGameSer[pGameSer->nSerNo])
				return false;

			g_pCenterServer->s_szGameSer[pGameSer->nSerNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szGameSer[pGameSer->nSerNo]);

			GameInfo game;
			game.szIp = pGameSer->szIp;
			game.nPort = pGameSer->nPort;
			game.nGameID = pGameSer->nGameID;

			g_pCenterServer->m_mapGameInfo[m_pNet->GetServiceIndex()] = game;

			BroadCastGameSerInfo(game);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandMainMsgFromConnSrv(USHORT nSrcIndex, USHORT nSub, void* pData, UINT nDataSize)
{
	switch(nSub)
	{
		case CS_SUB_MSG_CONN_SUCSS:
		{
			RegConnSer* pSer = (RegConnSer*)pData;
			if(0 != g_pCenterServer->s_szConnSer[pSer->nSerNo])
				return false;

			g_pCenterServer->s_szConnSer[pSer->nSerNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szConnSer[pSer->nSerNo]);

			SendAllGameSerInfo();
			return true;
		}
	}
	return false;
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
		if(0 == g_pCenterServer->s_szConnSer[i])
			continue;
		CNetSinkObj::SendData(m_pNet, g_pCenterServer->s_szConnSer[i], MAIN_MSG_CENTERSER, CT_SUB_MSG_NEWGAMESER, pData, nSize);
	}
}


void CCenSerSink::SendAllGameSerInfo()
{
	if(0 == g_pCenterServer->m_mapGameInfo.size())
		return;
	
	UINT nSize = sizeof(RegGameSer) * g_pCenterServer->m_mapGameInfo.size() + sizeof(USHORT);
	char* pData = new char[nSize];
	*(USHORT*)pData = g_pCenterServer->m_mapGameInfo.size();
	RegGameSer* pGame = (RegGameSer*)(pData+2);
	std::map<USHORT,GameInfo>::iterator it_game = g_pCenterServer->m_mapGameInfo.begin();
	for(; it_game != g_pCenterServer->m_mapGameInfo.end(); ++it_game)
	{
		memcpy(pGame->szIp, it_game->second.szIp.c_str(),strlen(it_game->second.szIp.c_str()));
		pGame->nPort = it_game->second.nPort;
		pGame->nGameID = it_game->second.nGameID;
		pGame->nSerNo = it_game->first;
		pGame += 1;
	}
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CENTERSER, CT_SUB_MSG_NEWGAMESER, pData, nSize);
}


