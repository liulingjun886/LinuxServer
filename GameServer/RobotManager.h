#pragma once
#include "../Defines.h"
#include <map>
#include "Robot.h"
#include <assert.h>
#include "../include/ToolLock.h"

class CRobotManager : public CServices
{
	Single(CRobotManager)
public:
	//void ActiveRobot(int num);
	//void RetrieveRobot();
public:
	//void Init(int num);
	bool HandData(int nType, SERVICEINDEX nSrcIndex, void *pData, DATASIZE nSize) { return true;}
private:
	CMutexLock m_metux;
};
