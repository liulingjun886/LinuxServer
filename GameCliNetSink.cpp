#include "GameCliNetSink.h"
#include <stdio.h>
#include "RoomManager.h"
#include "commproto.h"
#include "Room.h"
#include "Core.h"
#include "UserInfo.h"
#include "MemDataDef.h"
#include "NetSink.h"

extern USHORT g_serno;
extern UINT g_gameid;

CConnSerManager CGameCliNetSink::m_RemoteSer;
CGameCliNetSink::CGameCliNetSink(CServices* pNet) :CCliNetSink(pNet),m_nReConnectCount(0),m_nType(DATA_SER)
{
	m_timerConnTest.InitTimerObj(m_pNet, TIME_CONN_IS_LINK);
	m_timerReConn.InitTimerObj(m_pNet, TIME_CONN_RECONNECT);
}


CGameCliNetSink::~CGameCliNetSink()
{
	
}

void CGameCliNetSink::Init(UINT nIp)
{
	m_timerConnTest.StartTimerSec(5);
}

bool CGameCliNetSink::DisConnect()
{
	//printf("conn disconnect\n");
	//Single_Get(CCore)->Stop();

	if(m_nType == DATA_SER)
	{
		Single_Get(CCore)->Stop();
		DelDataSer();
	}
	else
	{
		m_RemoteSer.DelRemoteSer(g_serno, m_pNet->GetServiceIndex());
	}
	m_timerConnTest.StopTimer();
	m_timerReConn.StartTimerSec(5);
	return true;
}

bool CGameCliNetSink::HandNetData(USHORT nIndex,USHORT nMain, USHORT nSub, void* pData, USHORT nDataSize)
{
	m_nTestLink = 0;
	switch (nMain)
	{
	case MAIN_MSG_NET:
	{
		return HandMainMsgNet(nIndex,nSub,pData,nDataSize);
	} 
	case MAIN_MSG_CONNECT:
	{
		switch (nSub)
		{
			case SUB_MSG_USER4ROOM:
			{
				User2Room *pRoom = (User2Room *)pData;
				HandMainMsgRoom(pRoom->nIndex,pRoom->nSub, pRoom + 1, nDataSize-sizeof(User2Room));
				return true;
			}
			case SUB_MSG_USER4GAME:
			{
				User2Game* pGameRoom = (User2Game*)pData;
				m_pNet->PostData(pGameRoom->nIndex, NET_MSG ,pData , nDataSize);
				return true;
			}
			case SUB_MSG_USER_DOUBLELOGIN:
			{
				UserDoubleLogin* pDouble = (UserDoubleLogin*)pData;
				USHORT nIndex = m_RemoteSer.GetConnSerById(pDouble->nCid,m_pNet->GetServiceIndex());
				CNetSink::SendData(m_pNet, nIndex, MAIN_MSG_CONNECT, SUB_MSG_USER_DOUBLELOGIN,pData,nDataSize);
				return true;
			}
			case SUB_MSG_USER_RELOGIN:
			{
				User2Game* pGameRoom = (User2Game*)pData;
				if(!m_pNet->PostData(pGameRoom->nIndex, NET_MSG ,pData , nDataSize))
				{
					UserReConnInfo* pConn = (UserReConnInfo*)(pGameRoom+1);
					
					Mem::UserQuitGameReq req;
					req.nUserId = pConn->nUserId;
					CCliNetSink::PostMemDataBaseReq(m_pNet,Mem::USER_QUIT_GAME_REQ, &req, sizeof(Mem::UserQuitGameReq));
					
					InnserSync sync;
					sync.nIndex = pConn->nCsid;
					sync.nType = EN_RECONNECT_FAIL;
					USHORT nIndex = m_RemoteSer.GetConnSerById(pConn->nCid,m_pNet->GetServiceIndex());
					CNetSink::SendData(m_pNet,nIndex, MAIN_MSG_GAMESER, SUB_MSG_GAME2CONN,  &sync, sizeof(InnserSync));
				}
				return true;	
			}
			default:
				break;
		}
		break;
	}
	case MAIN_MSG_DATACENTER:
	{
		DataCenter* pCenter = (DataCenter*)(pData);
		if(SUB_MSG_DATA_BASE_RET == nSub)
		{
			m_pNet->PostData(pCenter->nCsid, DATA_BASE_RET, pCenter+1, nDataSize-sizeof(DataCenter));
		}
		else
		{
			m_pNet->PostData(pCenter->nCsid, MEM_DATA_BASE_RET, pCenter+1, nDataSize-sizeof(DataCenter));
		}
		break;
	}
	default:
		break;
	}
	return true;
}

bool CGameCliNetSink::HandTimeMsg(USHORT uTimeID)
{
	switch (uTimeID)
	{
	case TIME_CONN_IS_LINK:
	{
		CNetSink::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_TEST);
		m_timerConnTest.StartTimerSec(5);
		break;
	}
	case TIME_CONN_RECONNECT:
	{
		++m_nReConnectCount;
		if(m_nReConnectCount == 5)
		{
			return false;
		}
		m_pNet->PostData(m_pNet->GetServiceIndex(), NET_RECONNECT);
		m_timerReConn.StartTimerSec(5);
		break;
	}
	default:
		break;
	}
	return true;
}

bool CGameCliNetSink::HandMainMsgNet(USHORT nIndex, USHORT nSub, void* pData, USHORT nDataSize)
{
	switch (nSub)
	{
		case SUB_MSG_CONN_SUCSS:
		{
			m_timerReConn.StopTimer();
			m_nReConnectCount = 0;
			RegGameSer reg;
			reg.nGameID = g_gameid;
			reg.nSerNo = g_serno;
			CNetSink::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_REG_GAMESER, &reg, sizeof(RegGameSer));
			return true;
		}
		case SUB_MSG_REG_CONNSER:
		{
			RegConnSer* pReg = (RegConnSer*)pData;
			m_nType = CONN_SER;
			m_RemoteSer.AddRemoteSer(pReg->nSerNo, nIndex);
			//m_pNet->PriorityService();
			//m_pNet->SetTimer(TIME_CONN_IS_LINK, 6000, -1);
			return true;
		}
		case SUB_MSG_DATA_SUCSS:
		{
			AddDataSer();
			return true;
		}
		case SUB_MSG_TEST:
		{
			//printf("SUB_MSG_TEST\n");
			CNetSink::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_NET, SUB_MSG_TEST);
			return true;
		}
		default:
			break;
	}
	return false;
}

bool CGameCliNetSink::HandMainMsgRoom(USHORT nCsid,USHORT nSub, void* pData, USHORT nDataSize)
{
	UserBaseInfo* pUserInfo = (UserBaseInfo*)pData;

	switch (nSub)
	{
	case SUB_MSG_CREATE:
	{
		bool bRet(true);
		USHORT nIndex = 0;
		CreateRoom* pInfo = (CreateRoom*)(pUserInfo+1);
		do
		{
			
			CRoom* pRoom = new CRoom;
			if (!Single_Get(CCore)->AddService(pRoom))
			{
				bRet = false;
				break;
			}

			nIndex = pRoom->GetServiceIndex();
			int RoomId = Single_Get(CRoomManager)->AddRoom(nIndex);
			if (RoomId > 0 && pRoom->InitRoomData(pUserInfo->nUserId,RoomId, pInfo, nDataSize))
			{
				break;
			}
			
			bRet = false;
			m_pNet->PostData(nIndex, EXIT_MSG);
		}while(false);
		
		char buff[MAX_MSG_SIZE] = { 0 };
		if (bRet)
		{
			User2Game* pGame = (User2Game*)buff;
		   	pGame->nMain = MAIN_MSG_ROOM;
		   	pGame->nSub = SUB_MSG_JOIN;
		   	pGame->nIndex = nIndex;
			pGame->nSeatNo = nCsid;
			memcpy(pGame+1,pUserInfo,sizeof(UserBaseInfo));
		   	if(m_pNet->PostData(nIndex,NET_MSG, buff, sizeof(User2Game)+sizeof(UserBaseInfo)))
		   	{
				return true;
		   	}
		}

		Game2User* pGame = (Game2User*)buff;
		pGame->nIndex = nCsid;
		pGame->nMain = MAIN_MSG_ROOM_MANAGER;
		pGame->nSub = SUB_MSG_CREATE_FAIL;
		CNetSink::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_GAMESER, SUB_MSG_GAME4USER, pGame, sizeof(Game2User));
		return true;
	}
	case SUB_MSG_ENTER:
	{
		int nRoomId = *(int*)(pUserInfo+1);
		USHORT nIndex = Single_Get(CRoomManager)->GetRoom(nRoomId);

		char buff[MAX_MSG_SIZE] = { 0 };		
		if(nIndex != INVALID_SERIVCE_INDEX)
		{
			User2Game* pCom = (User2Game*)buff;
			pCom->nMain = MAIN_MSG_ROOM;
			pCom->nSub = SUB_MSG_JOIN;
			pCom->nSeatNo = nCsid;
			pCom->nIndex = nIndex;
			memcpy(pCom+1,pUserInfo,sizeof(UserBaseInfo));
			if(m_pNet->PostData(nIndex, NET_MSG, pCom,sizeof(UserBaseInfo)+sizeof(User2Game)))
			{
				return true;
			}
		}
		
		Game2User* pGame = (Game2User*)buff;
		pGame->nIndex = nCsid;
		pGame->nMain = MAIN_MSG_ROOM_MANAGER;
		pGame->nSub = SUB_MSG_JOIN_FAIL;
		CNetSink::SendData(m_pNet,m_pNet->GetServiceIndex(), MAIN_MSG_GAMESER, SUB_MSG_GAME4USER, buff, sizeof(Game2User));
		return true;
	}
	default:
		break;
	}
	return true;
}

USHORT CGameCliNetSink::GetConnSerById(USHORT nCid,USHORT nIndex)
{
	return m_RemoteSer.GetConnSerById(nCid,nIndex);
}
