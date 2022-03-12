#pragma once
#include "ToolLock.h"
#include "ObjectManager.h"

template<class T,bool usePool=false> class CSingleObject
{
	CSingleObject(){}
	CSingleObject(const CSingleObject& obj){}
	CSingleObject& operator=(const CSingleObject& obj){}
	static T* _instance;
	static CMutexLock _mutex;
public:
	static T* Instance()
	{
		if(NULL == _instance)
		{
			CToolLock lock(&_mutex);
			if(NULL == _instance)
			{
				if(usePool)
				{
					_instance = CObjctManager<T>::AllocateIntance();
				}
				else
				{
					_instance = CObjctManager<T>::CreateIntance();
				}
			}
		}
		return _instance;
	}
	
	static void Destroy()
	{
		if(NULL != _instance)
		{
			CToolLock lock(&_mutex);
			if(NULL != _instance)
			{
				if(usePool)
				{
					CObjctManager<T>::DeAllocateIntance(_instance);
				}
				else
				{
					CObjctManager<T>::DeleteIntance(_instance);
				}
				_instance = NULL;
			}
		}
	}
	
};
template<class T,bool usePool> T* CSingleObject<T,usePool>::_instance = NULL;
template<class T,bool usePool> CMutexLock CSingleObject<T,usePool>::_mutex;