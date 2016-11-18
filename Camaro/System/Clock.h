#pragma once

#include <Common/Common.h>
#include <time.h>

#define CLOCK GOD::m_clock

#define CLOCK_DATE_STRING_SIZE 12

#define CLOCK_TICKS_TIMESTAMP(name) static clock_t name = clock()
#define CLOCK_CURRENT_TICKS(name) const clock_t name = clock()

#define CLOCK_GAMETICKS_TIMESTAMP(name) static clock_t name = CLOCK->GetGameTimeInTicks()
#define CLOCK_CURRENT_GAMETICKS(name) const clock_t name = CLOCK->GetGameTimeInTicks()

#define CLOCK_TICKS_AS_MS(duration) ((clock_t)(duration * (CLOCKS_PER_SEC / 1000)))
#define CLOCK_ELAPSED_TICKS(last, now) (now - last)
#define CLOCK_ELAPSED_TICKS_IN_MS(last, now) ((float)(now - last) / (float)CLOCKS_PER_SEC)

class CClock
{
public:
	CClock();

	void CurrentTimeAsString(char* destination) const;
	void UpdateGameTime();
	float GetGameTimeInSeconds() { return (float)GetGameTimeInTicks() / (float)CLOCKS_PER_SEC; } const
	clock_t GetGameTimeInTicks() const { return m_gameTime; }
	void ResetGameTime() { m_lastGameTimeStamp = clock(); }

	~CClock() { ; }

private:
	clock_t m_lastGameTimeStamp;
	clock_t m_gameTime;
};