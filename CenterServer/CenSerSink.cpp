#include "CenSerSink.h"
#include "../NetSinkObj.h"
#include "../include/Services.h"

extern CCenterServer* g_pCenterServer;

enum TIMER_ID
{
	TIME_TEST_LINK = 1,
};

CCenSerSink::CCenSerSink(CServices* pServices):CNetHandSink(pServices)
{
	m_timer_Link.InitTimerObj(pServices, TIME_TEST_LINK);
}

CCenSerSink::~CCenSerSink()
{
	
}

void CCenSerSink::Connect()
{
	m_pNet->Log("socket connected !");
	m_nTestNum = 0;
	ConnSucess conn;
	conn.nSrvNo = g_pCenterServer->GetSerNo();
	conn.nSrvType = g_pCenterServer->GetSerType();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CENTERSER, CT_SUB_MSG_CONN_SUCSS,&conn,sizeof(conn));
}

void CCenSerSink::Close()
{
	m_pNet->Log("socket closed !");
	
	std::map<uint16,uint16*>::iterator it = g_pCenterServer->m_mapLinkInfo.find(m_pNet->GetServiceIndex());
	if(it == g_pCenterServer->m_mapLinkInfo.end())
		return;

	*(it->second) = 0;
	g_pCenterServer->m_mapLinkInfo.erase(it);

	std::map<uint16,GameInfo>::iterator it_game = g_pCenterServer->m_mapGameInfo.find(m_pNet->GetServiceIndex());
	if(it_game != g_pCenterServer->m_mapGameInfo.end())
	{
		g_pCenterServer->m_mapGameInfo.erase(it_game);
	}
	return;
}


bool CCenSerSink::HandNetData(uint16 nSrcIndex, uint16 nMain, uint16 nSub, void* pData, DATASIZE nDataSize)
{
	m_pNet->Log("Recv cmd %d, %d", nMain, nSub);
	switch(nMain)
	{
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

bool CCenSerSink::HandTimeMsg(uint16 nTimeID)
{
	switch(nTimeID)
	{
		case TIME_TEST_LINK:
		{
			++m_nTestNum;
			if(m_nTestNum > 1)
			{
				m_pNet->Log("test link timeout!");
				return false;
			}
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandTestNetConn()
{
	m_nTestNum = 0;
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CENTERSER, CT_SUB_MSG_TEST);
	return true;
}


bool CCenSerSink::HandMainMsgFromUserSrv(uint16 nSrcIndex, uint16 nSub, void* pData, DATASIZE nDataSize)
{
	switch(nSub)
	{
		case US_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
		case US_SUB_MSG_REGUSERSRV:
		{
			RegConnSer* pSer = (RegConnSer*)pData;
			if(0 != g_pCenterServer->s_szUserSer[pSer->nSerNo])
				return false;
			g_pCenterServer->s_szConnSer[pSer->nSerNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szConnSer[pSer->nSerNo]);
			HandTestNetConn();
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandMainMsgFromDataSrv(uint16 nSrcIndex, uint16 nSub, void* pData, DATASIZE nDataSize)
{
	switch(nSub)
	{
		case DS_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
		case DS_SUB_MSG_REG_DATASRV:
		{
			RegConnSer* pSer = (RegConnSer*)pData;
			if(0 != g_pCenterServer->s_szDataSer[pSer->nSerNo])
				return false;

			g_pCenterServer->s_szDataSer[pSer->nSerNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szDataSer[pSer->nSerNo]);
			HandTestNetConn();
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandMainMsgFromGameSrv(uint16 nSrcIndex, uint16 nSub, void* pData, DATASIZE nDataSize)
{
	switch(nSub)
	{
		case GS_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
		case GS_SUB_MSG_REG_GAMESRV:
		{
			RegGameSer* pGameSer = (RegGameSer*)pData;
			if(0 != g_pCenterServer->s_szGameSer[pGameSer->nSerNo])
				return false;

			g_pCenterServer->s_szGameSer[pGameSer->nSerNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szGameSer[pGameSer->nSerNo]);

			GameInfo game;
			game.szIp = pGameSer->szIp;
			game.nPort = pGameSer->nPort;
			game.nGameId = pGameSer->nGameID;
			g_pCenterServer->m_mapGameInfo[m_pNet->GetServiceIndex()] = game;
			HandTestNetConn();
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);

			BroadCastGameSerInfo(game);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandMainMsgFromConnSrv(uint16 nSrcIndex, uint16 nSub, void* pData, DATASIZE nDataSize)
{
	switch(nSub)
	{
		case CS_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
		case CS_SUB_MSG_REG_CONN:
		{
			RegConnSer* pSer = (RegConnSer*)pData;
			//if(0 != g_pCenterServer->s_szConnSer[pSer->nSerNo])
			//{
			//	m_pNet->Log("Connect %d Have Reged", pSer->nSerNo);
			//	return false;
			//}

			g_pCenterServer->s_szConnSer[pSer->nSerNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szConnSer[pSer->nSerNo]);
			HandTestNetConn();
			SendAllGameSerInfo();
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);
			return true;
		}
	}
	return false;
}



void CCenSerSink::BroadCastGameSerInfo(const GameInfo& gameInfo)
{
	/*uint32 nSize = sizeof(GameInfo) + sizeof(uint16);
	char* pData = new char[sizeof(GameInfo) + sizeof(uint16)];
	*(uint16*)pData = 1;
	GameInfo* pGame = (GameInfo*)(pData+2);
	memcpy(pGame->szIp, gameInfo.szIp.c_str(),strlen(gameInfo.szIp.c_str()));
	pGame->nPort = gameInfo.nPort;
	pGame->nGameId = gameInfo.nGameId;

	for(int i = 0; i < 500; i++)
	{
		if(0 == g_pCenterServer->s_szConnSer[i])
			continue;
		CNetSinkObj::SendData(m_pNet, g_pCenterServer->s_szConnSer[i], MAIN_MSG_CENTERSER, CT_SUB_MSG_NEWGAMESER, pData, nSize);
	}*/
}


void CCenSerSink::SendAllGameSerInfo()
{
	if(0 == g_pCenterServer->m_mapGameInfo.size())
		return;
	
	uint32 nSize = sizeof(RegGameSer) * g_pCenterServer->m_mapGameInfo.size() + sizeof(uint16);
	char* pData = new char[nSize];
	*(uint16*)pData = g_pCenterServer->m_mapGameInfo.size();
	RegGameSer* pGame = (RegGameSer*)(pData+2);
	std::map<uint16,GameInfo>::iterator it_game = g_pCenterServer->m_mapGameInfo.begin();
	for(; it_game != g_pCenterServer->m_mapGameInfo.end(); ++it_game)
	{
		memcpy(pGame->szIp, it_game->second.szIp.c_str(),strlen(it_game->second.szIp.c_str()));
		pGame->nPort = it_game->second.nPort;
		pGame->nGameID = it_game->second.nGameId;
		pGame->nSerNo = it_game->first;
		pGame += 1;
	}
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CENTERSER, CT_SUB_MSG_NEWGAMESER, pData, nSize);
}


