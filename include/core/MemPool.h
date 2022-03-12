#pragma once
#include "types.h"
struct CMemPool
{
	static void* Allocate(uint32 nSize);
	static void DeAllocate(void* pPtr);
};