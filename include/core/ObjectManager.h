#pragma once
#include "MemPool.h"

template<class T> struct CObjctManager
{
	static T* AllocateIntance()
	{
		void* ptr = CMemPool::Allocate(sizeof(T));
		if(ptr)
			return new (ptr) T;
		return NULL;
	}
	
	static void DeAllocateIntance(T* p)
	{
		if(!p)
			return;
		p->~T();
		CMemPool::DeAllocate(p);
	}
	
	static T* CreateIntance()
	{
		try
		{
			return new T;
		}
		catch(...)
		{
			return NULL;
		}
	}
	
	static void DeleteIntance(T* p)
	{
		if(p)
			delete p;
	}
};