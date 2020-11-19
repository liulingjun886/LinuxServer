#pragma once
#include "types.h"
#include <map>
#include "Robot.h"
class CRobotManager : public CServices
{
	Single(CRobotManager)
public:
	void ActiveRobot(int num);
	void RetrieveRobot();
public:
	void Init(int num);
private:
	CMutexLock m_metux;
};
