#ifndef _CLOG_H_
#define _CLOG_H_

#include "Common/Common.h"

#define LogWarning GOD::m_log->Warning
#define LogError GOD::m_log->Error
#define LogFatalError GOD::m_log->FatalError
#define LogAssertViolation GOD::m_log->AssertViolation
#define LogInfo GOD::m_log->Info

class CLog
{
public:
	CLog() { ; }
	~CLog() { ; }

	void Warning(const char* string, ...);
	void Error(const char* string, ...);
	void FatalError(const char* string, ...);
	void AssertViolation(const char* string, ...);
	void Info(const char* string, ...);

private:
	void WriteMessage(const char* title, const char* msg);
};

#endif // _CLOG_H_