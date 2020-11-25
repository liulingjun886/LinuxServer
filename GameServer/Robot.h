#pragma once
#include "../include/Services.h"
class CRobot :
	public CServices
{
	friend class CRobotManager;
private:
	CRobot();
	~CRobot();
};

