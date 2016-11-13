#ifndef _THREAD_H_
#define _THREAD_H_

#include "../God.h"

#ifdef USE_THREADING

#	include <process.h>

#define THREAD_MAX_NAME_LENGTH 32
#define THREAD_DEFAULT_STACKSIZE 65536

// Usage:
// 1. Inherit from CThread class
// 2. Call parent constructor in child constructor : CThread("ThreadName")
// 3. Implement virtual RunLoop() - returning false will shutdown thread
// 4. Call StartThread() to run thread
// 5. Call ForceSafeShutdownThread() in destructor

class CThread
{
protected:
	CThread();
	CThread(char* threadName);
	virtual ~CThread();
	virtual bool RunLoop() = 0;
	char* GetThreadName() const { return m_threadName; }
	uint GetThreadID() const { return m_threadID; }
	bool IsGoingToShutdown() const { return m_shutdownRequested; }
	void StartThread();
	void ForceSafeShutdownThread();

private:
	uint m_threadID;
	char* m_threadName;
	HANDLE m_handle;
	bool m_shutdownRequested;

	void SetThreadName(uint dwThreadID, char* szThreadName);
	static unsigned __stdcall func(void *args);
};

#endif

#endif // _THREAD_H_