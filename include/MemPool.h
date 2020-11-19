#pragma once

#include <map>
#include <list>
#include <set>

#include "ToolLock.h"

class CMemPool
{
public:
    CMemPool(unsigned int min = 10);
    ~CMemPool();

    /*
    * 功能:分配内存
    * 参数说明:
    *IN size:所需要的内存大小
    *OUT result_size:实际分配的内存大小
    *返回值:
    * 成功:分配内存的地址,失败:NULL
    */
    void* allocate(unsigned int size, unsigned int & result_size);

    /*
    * 功能:回收内存
    * 参数说明:
    * IN mem:待回收的内存地址
    * IN mem_size: 内存大小
    * 返回值:
    * 成功:0,失败:非0
    */
    int recycle(void* mem, unsigned mem_size);
    
private:
    int extend(unsigned int size, std::list<void*>* l, std::set<void*>* s);
    int extend_new_size(unsigned int size);
    int release(unsigned int size, std::list<void*>* l, std::set<void*>* s);

    unsigned int release_size(unsigned int block_size, unsigned int free_count, unsigned int stub_count);
    unsigned int fit_block_size(unsigned int size);
    unsigned int fit_extend_set(unsigned size);
    int release_size(unsigned int mem_size);
	
private:
    typedef std::map<unsigned int, std::list<void*>*> mml;
    typedef std::map<unsigned int, std::set<void*>*> mms;
    //空闲内存链表
    mml _free;
    //内存块存根
    mms _stub;
    //已分配内存总大小
    unsigned int _allocated_size;
    //内存分配最大水位
    unsigned int _water_mark;
	//内存分配最小值
	const unsigned int _min;
};


class CLockMemPool
{
public:
	CLockMemPool(unsigned int min = 10):m_memPool(min){}
	~CLockMemPool(){}
public:
	void* allocate(unsigned int size, unsigned int & result_size)
	{
		CToolLock lock(&m_mem_mutex);
		return m_memPool.allocate(size,result_size);
	}
    int recycle(void* mem, unsigned mem_size)
	{
		CToolLock lock(&m_mem_mutex);
		return m_memPool.recycle(mem, mem_size);
    }
private:
	CMemPool m_memPool;
	CMutexLock m_mem_mutex;
};