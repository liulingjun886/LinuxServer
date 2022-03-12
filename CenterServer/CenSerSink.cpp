#include "CenSerSink.h"
#include "../NetSinkObj.h"
#include "../include/core/Services.h"


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
	COutputPacket out;
	out.Begin(MAIN_MSG_CENTERSER, CT_SUB_MSG_CONN_SUCSS);
	out.WriteInt16(g_pCenterServer->GetSerNo());
	out.WriteInt16(g_pCenterServer->GetSerType());
	out.End();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(),out);
	
	//ConnSucess conn;
	//conn.nSrvNo = g_pCenterServer->GetSerNo();
	//conn.nSrvType = g_pCenterServer->GetSerType();
	//CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), MAIN_MSG_CENTERSER, CT_SUB_MSG_CONN_SUCSS,&conn,sizeof(conn));
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

bool CCenSerSink::HandNetData(uint16 nMain, uint16 nSub, CInputPacket& InPack)
{
	m_pNet->Log("Recv cmd %d, %d", nMain, nSub);
	switch(nMain)
	{
		case MAIN_MSG_CONNSER:
		{
			return HandMainMsgFromConnSrv(nSub, InPack);
		}
		case MAIN_MSG_GAMESER:
		{
			return HandMainMsgFromGameSrv(nSub, InPack);
		}
		case MAIN_MSG_USERSER:
		{
			return HandMainMsgFromUserSrv(nSub, InPack);
		}
		case MAIN_MSG_DATASER:
		{
			return HandMainMsgFromDataSrv(nSub, InPack);
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
	COutputPacket out;
	out.Begin(MAIN_MSG_CENTERSER, CT_SUB_MSG_TEST);
	out.End();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(),out);
	return true;
}


bool CCenSerSink::HandMainMsgFromUserSrv(uint16 nSub, CInputPacket& inPacket)
{
	switch(nSub)
	{
		case US_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
		case US_SUB_MSG_REGUSERSRV:
		{
			uint16 nSrvNo = inPacket.ReadInt16();
			if(0 != g_pCenterServer->s_szUserSer[nSrvNo])
				return false;
			g_pCenterServer->s_szConnSer[nSrvNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szConnSer[nSrvNo]);
			HandTestNetConn();
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandMainMsgFromDataSrv(uint16 nSub, CInputPacket& inPacket)
{
	switch(nSub)
	{
		case DS_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
		case DS_SUB_MSG_REG_DATASRV:
		{
			uint16 nSrvNo = inPacket.ReadInt16();
			if(0 != g_pCenterServer->s_szDataSer[nSrvNo])
				return false;

			g_pCenterServer->s_szDataSer[nSrvNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szDataSer[nSrvNo]);
			HandTestNetConn();
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandMainMsgFromGameSrv(uint16 nSub, CInputPacket& inPacket)
{
	switch(nSub)
	{
		case GS_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
		case GS_SUB_MSG_REG_GAMESRV:
		{
			uint16 nSrvNo = inPacket.ReadInt16();
			if(0 != g_pCenterServer->s_szGameSer[nSrvNo])
				return false;

			g_pCenterServer->s_szGameSer[nSrvNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szGameSer[nSrvNo]);

			GameInfo game;
			game.nGameId = inPacket.ReadInt32();
			game.szIp = inPacket.ReadString();
			game.nPort = inPacket.ReadInt16();
			g_pCenterServer->m_mapGameInfo[m_pNet->GetServiceIndex()] = game;
			HandTestNetConn();
			m_timer_Link.StartTimerSec(SERVER_TEST_TIME);

			BroadCastGameSerInfo(game);
			return true;
		}
	}
	return false;
}

bool CCenSerSink::HandMainMsgFromConnSrv(uint16 nSub, CInputPacket& inPacket)
{
	switch(nSub)
	{
		case CS_SUB_MSG_TEST:
		{
			return HandTestNetConn();
		}
		case CS_SUB_MSG_REG_CONN:
		{
			uint16 nSerNo = inPacket.ReadInt16();
			g_pCenterServer->s_szConnSer[nSerNo] = m_pNet->GetServiceIndex();
			g_pCenterServer->m_mapLinkInfo[m_pNet->GetServiceIndex()] = &(g_pCenterServer->s_szConnSer[nSerNo]);
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

	COutputPacket out;
	out.Begin(MAIN_MSG_CENTERSER, CT_SUB_MSG_NEWGAMESER);
	out.WriteInt16((uint16)g_pCenterServer->m_mapGameInfo.size());
	
	std::map<uint16,GameInfo>::iterator it_game = g_pCenterServer->m_mapGameInfo.begin();
	for(; it_game != g_pCenterServer->m_mapGameInfo.end(); ++it_game)
	{
		out.WriteInt16(it_game->first);
		out.WriteString(it_game->second.szIp);
		out.WriteInt16(it_game->second.nPort);
		out.WriteInt32(it_game->second.nGameId);
	}
	out.End();
	CNetSinkObj::SendData(m_pNet, m_pNet->GetServiceIndex(), out);
}


