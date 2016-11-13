#include "Log.h"

#include <cstdio>
#include <stdarg.h>

#include "God.h"

void CLog::Warning(const char* string, ...)
{
	char* msg;
	va_list arg;
	va_start(arg, string);
	int length = _vscprintf(string, arg) + 1;
	msg = (char*)malloc(length * sizeof(char));
	vsprintf_s(msg, length, string, arg);
	va_end(arg);

	WriteMessage("[Warning] ", msg);

	free(msg);
}

void CLog::Error(const char* string, ...)
{
	char* msg;
	va_list arg;
	va_start(arg, string);
	int length = _vscprintf(string, arg) + 1;
	msg = (char*)malloc(length * sizeof(char));
	vsprintf_s(msg, length, string, arg);
	va_end(arg);

	WriteMessage("[Error] ", msg);

	free(msg);
}

void CLog::FatalError(const char* string, ...)
{
	char* msg;
	va_list arg;
	va_start(arg, string);
	int length = _vscprintf(string, arg) + 1;
	msg = (char*)malloc(length * sizeof(char));
	vsprintf_s(msg, length, string, arg);
	va_end(arg);

	WriteMessage("[FatalError] ", msg);

	free(msg);
	__debugbreak();
}

void CLog::AssertViolation(const char* string, ...)
{
	char* msg;
	va_list arg;
	va_start(arg, string);
	int length = _vscprintf(string, arg) + 1;
	msg = (char*)malloc(length * sizeof(char));
	vsprintf_s(msg, length, string, arg);
	va_end(arg);

	WriteMessage("[AssertViolation] ", msg);

	free(msg);
	__debugbreak();
}

void CLog::Info(const char* string, ...)
{
	char* msg;
	va_list arg;
	va_start(arg, string);
	int length = _vscprintf(string, arg) + 1;
	msg = (char*)malloc(length * sizeof(char));
	vsprintf_s(msg, length, string, arg);
	va_end(arg);

	WriteMessage("[Info] ", msg);

	free(msg);
}

void CLog::WriteMessage(const char* title, const char* msg)
{
	char date[CLOCK_DATE_STRING_SIZE];
	CLOCK->CurrentTimeAsString(date);

	char memory[MEMORY_STRING_SIZE];
	MEM_ALLOCATOR->CurrentUsedMemoryAsString(memory);

	char* final_msg;
	// CLOCK_DATE_STRING_SIZE(includes '\0') + MEMORY_STRING_SIZE + title + msg +\n
	int length = CLOCK_DATE_STRING_SIZE + MEMORY_STRING_SIZE + strlen(title) + strlen(msg) + 1;
	final_msg = (char*)malloc(length * sizeof(char));

	strcpy_s(final_msg, length, date);
	strcat_s(final_msg, length, memory);
	strcat_s(final_msg, length, title);
	strcat_s(final_msg, length, msg);
	strcat_s(final_msg, length, "\n");

#ifdef USE_DXRENDERER
	OutputDebugString(final_msg);
#else
	printf(final_msg);
#endif

	static bool cleanLog = true;

	// TOD: THIS IS SUPER SLOW, THE FILE HANDLE SHOULD KEPT OPEN AND JUST BE CLEARED EVERY START - extra class for that
	FILE * file;
	const char* mode = "a";
	if (cleanLog)
	{
		mode = "w";
		cleanLog = false;
	}

	if (fopen_s(&file, "log.txt", mode) != 0)
	{
		printf("[WriteError] Could not open log file\n");
		return;
	}

	if (file != NULL)
	{
		fputs(final_msg, file);
		fclose(file);
	}

	free(final_msg);
}
