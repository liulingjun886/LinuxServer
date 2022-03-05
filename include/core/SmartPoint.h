#pragma once
#include "types.h"
#include <assert.h>
#include <stdio.h>
#include <errno.h>

template<class T> class CSmartPoint
{
	T* m_pData;
public:
	CSmartPoint() :m_pData(new T)
	{

	}
	CSmartPoint(T* pData) :m_pData(pData)
	{

	}
	~CSmartPoint()
	{
		SAFE_DELTE(m_pData);
	}
public:
	T& operator* ()
	{
		return *m_pData;
	}

	T* operator->()
	{
		return m_pData;
	}
};

template<class T>class CSmartArrayPoint
{
	unsigned int m_uNum;
	T* m_pData;
public:
	CSmartArrayPoint() :m_uNum(0), m_pData(NULL)
	{

	}

	CSmartArrayPoint(unsigned int uNum) :m_uNum(uNum), m_pData(NULL)
	{
		m_pData = new T[m_uNum];
	}

	~CSmartArrayPoint()
	{
		SAFE_DELTEARRAY(m_pData);
	}

	void Init(unsigned int uNum)
	{
		if (m_uNum != 0)
		{
			if(m_pData)
			{
				delete[] m_pData;
			}
			m_uNum = 0;
			m_pData = NULL;
		}
		m_uNum = uNum;
		m_pData = new T[m_uNum];
	}

	void Reset(char nData)
	{
		if(m_uNum == 0)
			return;

		memset(m_pData,nData,sizeof(T)*m_uNum);
	}

	T& operator[] (unsigned int nIndex)
	{
		assert(nIndex < m_uNum);
		return m_pData[nIndex];
	}

	const unsigned int size() const
	{
		return m_uNum;
	}
};