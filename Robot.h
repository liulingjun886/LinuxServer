#pragma once
#include "Services.h"
class CRobot :
	public CServices
{
	friend class CRobotManager;
private:
	CRobot();
	~CRobot();
};

