#pragma once

#include <Common/Common.h>

#define MEMORY_STRING_SIZE 16
#define MEM_ALLOCATOR GOD::m_memoryAllocator

void* custom_malloc(size_t size);
#define malloc(size) custom_malloc(size)

void custom_free(void* p);
#define free(p) custom_free(p)

class CMemoryAllocator
{
public:
	CMemoryAllocator();
	~CMemoryAllocator();

	static CMemoryAllocator* Initialize();
	bool IsActive() const { return m_isActive; }
	void AddToTotalMemUsed(size_t size);
	void RemoveFromTotalMemUsed(size_t size);
	size_t GetMemoryUsed() const { return m_memoryUsed; }
	void CurrentUsedMemoryAsString(char* destination) const;

protected:
	static bool m_isActive;
	static size_t m_memoryUsed;
	static uint m_allocationsCount;

};