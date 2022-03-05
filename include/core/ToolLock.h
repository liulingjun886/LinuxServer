#pragma once
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>

struct ILock
{
	virtual void Lock(int nType=0) = 0;
	virtual void UnLock() = 0;
};


using namespace std;
class CMutexLock : public ILock
{
	pthread_mutex_t m_metux;
public:
	CMutexLock();
	~CMutexLock();
public:
	virtual void Lock(int nType=1);
	virtual void UnLock();
};

class CRWLock : public ILock
{
	pthread_rwlock_t m_rwlock;
public:
	CRWLock();
	~CRWLock();
public:
	virtual void Lock(int nType = 0);
	virtual void UnLock();
};

class CToolLock
{
	ILock* m_pLock;
	const bool m_bAutoLock;
	const int m_nLock_Type;
public:
	CToolLock(ILock* pLock, int nLock_Type = 0, bool bAutoLock = true) :m_pLock(pLock),m_bAutoLock(bAutoLock),m_nLock_Type(nLock_Type)
	{
		if (m_pLock && m_bAutoLock)
			m_pLock->Lock(m_nLock_Type);
	}
	~CToolLock()
	{
		if (m_pLock && m_bAutoLock)
			m_pLock->UnLock();
	}
public:
	void Lock(int nLock_Type=0);
	void UnLock();
};
