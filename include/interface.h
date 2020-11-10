#pragma once
#include "types.h"

struct IDataBase
{
	virtual ~IDataBase(){}
	virtual bool Init() = 0;
	virtual bool OpenConnection() = 0;
	virtual void CloseConnection() = 0;

	virtual void SetSpName(const    char* szSpName) = 0;
	virtual bool ExecPro() = 0;
	virtual bool ExecSql(const char* szSql) = 0;
	
	virtual bool HaveNext() = 0;
	virtual void MoveNext() = 0;
	
	virtual long long GetNumValue(const char* szFild,long long nDef = 0) = 0;
	virtual const char* GetStrValue(const char* szFild,const char* szDef = NULL) = 0;
	virtual long long GetOutNumValue(const char* szStrFild,long long nDef = 0) = 0;
	virtual const char* GetOutStrValue(const char* szFild,const char* szDef = NULL) = 0;

	virtual void AddNumParam(long long llValue) = 0;
	virtual void AddStrParam(const char* szValue) = 0;
	virtual void AddOutParam(const     char* szOutParam) = 0;
};
