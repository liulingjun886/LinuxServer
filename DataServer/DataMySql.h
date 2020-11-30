#pragma once
#include "../include/interface.h"
#include "../DataBaseEnginer/MySql.h"

using namespace std;
class CServices;

class CDataMySql : public CMySql
{
public:
	CDataMySql(CServices* pService);
	~CDataMySql();
public:
	virtual int  Exec(SERVICEINDEX nIndex, SERVICEINDEX nCsid ,uint32 nType,void* pData,uint32 nDataSize);
protected:
	virtual int  GetDBConfig();
	virtual bool Connected();
private:
	void UserLogin(uint16 nIndex,SERVICEINDEX nCsid,void* pData,uint16 nDataSize);
};
