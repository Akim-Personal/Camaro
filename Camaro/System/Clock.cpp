#include "Clock.h"

#include "God.h"

#define GAMETIME_UPDATE_TIMEOUT CLOCK_TICKS_AS_MS(1000)

CClock::CClock()
{
	m_lastGameTimeStamp = 0;
	m_gameTime = 0;
}

void CClock::CurrentTimeAsString(char* destination) const
{
	tm localTime;
	time_t currentTime = time(NULL);

	localtime_s(&localTime, &currentTime);
	sprintf_s(destination, CLOCK_DATE_STRING_SIZE, "<%02d:%02d:%02d> ", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
}

void CClock::UpdateGameTime()
{
	CLOCK_CURRENT_TICKS(ct);
	clock_t delta = CLOCK_ELAPSED_TICKS(m_lastGameTimeStamp, ct);

	if (delta > GAMETIME_UPDATE_TIMEOUT)
	{
		LogWarning("Long frame time detected - Game time update will be skipped for this frame");
		m_lastGameTimeStamp = clock();
	}
	else
	{
		m_gameTime += delta;
		m_lastGameTimeStamp = ct;
	}
}