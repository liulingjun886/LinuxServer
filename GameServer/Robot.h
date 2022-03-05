#pragma once
#include "../include/core/Services.h"
class CRobot :
	public CServices
{
	friend class CRobotManager;
private:
	CRobot();
	~CRobot();
};

