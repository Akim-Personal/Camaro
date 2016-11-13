#include "Thread.h"

#ifdef USE_THREADING

// More Info: http://msdn.microsoft.com/en-us/library/windows/desktop/ee416321%28v=vs.85%29.aspx

CThread::CThread()
{
	m_threadID = 0;
	m_shutdownRequested = false;
	m_threadName= "UnknownThread";
	m_handle = (HANDLE)_beginthreadex(NULL, THREAD_DEFAULT_STACKSIZE, CThread::func, this, CREATE_SUSPENDED, &m_threadID);
	ASSERT(m_handle);
	LogInfo("Thread \"%s\" has been created ...", m_threadName);
}

CThread::CThread(char* threadName)
{
	m_threadID = 0;
	m_shutdownRequested = false;
	m_threadName= threadName? threadName : "UnknownThread";
	m_handle = (HANDLE)_beginthreadex(NULL, THREAD_DEFAULT_STACKSIZE, CThread::func, this, CREATE_SUSPENDED, &m_threadID);
	ASSERT(m_handle);
	LogInfo("Thread \"%s\" has been created ...", m_threadName);
}

CThread::~CThread()
{
	CloseHandle(m_handle);
}

void CThread::ForceSafeShutdownThread()
{
	m_shutdownRequested = true;
	WaitForSingleObject(m_handle, INFINITE);
}

void CThread::StartThread()
{
	SetThreadName(m_threadID, m_threadName);
	ResumeThread(m_handle);
	LogInfo("Thread \"%s\" has been started ...", m_threadName);
}

void CThread::SetThreadName(uint dwThreadID, char* szThreadName)
{
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType;     // must be 0x1000
		LPCSTR szName;    // pointer to name (in user address space)
		DWORD dwThreadID; // thread ID (-1 = caller thread)
		DWORD dwFlags;    // reserved for future use, must be zero
	} THREADNAME_INFO;

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = szThreadName;
	info.dwThreadID = (DWORD)dwThreadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD),	(DWORD*)&info );
	}
	__except(EXCEPTION_CONTINUE_EXECUTION) {
	}
}

unsigned __stdcall CThread::func(void *args)
{
	CThread* currentThread = (CThread*)args;
	while (!currentThread->IsGoingToShutdown() && currentThread->RunLoop()) {}

	LogInfo("Thread \"%s\" has shut down", currentThread->GetThreadName());

	_endthreadex(0);
	return 0;
}

#endif