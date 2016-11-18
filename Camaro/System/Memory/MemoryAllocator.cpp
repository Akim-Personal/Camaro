#include "MemoryAllocator.h"

#include <System/God.h>

bool CMemoryAllocator::m_isActive = false;
size_t CMemoryAllocator::m_memoryUsed = 0;
uint CMemoryAllocator::m_allocationsCount = 0;

#undef malloc
void* custom_malloc(size_t size)
{
	if (MEM_ALLOCATOR && MEM_ALLOCATOR->IsActive())
		MEM_ALLOCATOR->AddToTotalMemUsed(size);
	return malloc(size);
}

#undef free
void custom_free(void* p)
{
	size_t size = _msize(p);
	if (MEM_ALLOCATOR && MEM_ALLOCATOR->IsActive())
		MEM_ALLOCATOR->RemoveFromTotalMemUsed(size);
	free(p);
}

void* operator new(size_t size)
{
	return custom_malloc(size);
}

void* operator new[](size_t size)
{
	return custom_malloc(size);
}

void operator delete(void* p)
{
	custom_free(p);
}

void operator delete[](void* p)
{
	custom_free(p);
}

CMemoryAllocator::CMemoryAllocator()
{
	m_isActive = true;
}

void CMemoryAllocator::AddToTotalMemUsed(size_t size)
{
	m_memoryUsed += size;
	++m_allocationsCount;
}

void CMemoryAllocator::RemoveFromTotalMemUsed(size_t size)
{
	m_memoryUsed -= size;
	--m_allocationsCount;
}

void CMemoryAllocator::CurrentUsedMemoryAsString(char* destination) const
{
	sprintf_s(destination, MEMORY_STRING_SIZE, "<%.2fMB> ", m_memoryUsed / (1024.0f * 1024.0f));
}

CMemoryAllocator::~CMemoryAllocator()
{
	if (m_memoryUsed > 0 || m_allocationsCount > 0)
		__debugbreak();

	m_isActive = false;
}