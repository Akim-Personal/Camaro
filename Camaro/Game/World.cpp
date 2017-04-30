#include "World.h"	

#include <Input/InputManager.h>
#include "InstanceManager.h"

#ifdef USE_BOX2D
#	include <Physics/Physics.h>
#endif

#define FPS_UPDATE_INTERVAL CLOCK_TICKS_AS_MS(500)

CWorld::CWorld()
	: CThread("WorldThread")
{
	m_inputManager = NULL;
	m_instanceManager = NULL;
}

bool CWorld::Init()
{
	m_fps = 0.0f;
	m_isPaused = false;

	m_inputManager = new CInputManager();
	m_instanceManager = new CInstanceManager();
	m_instanceManager->Init();

	CLOCK->ResetGameTime();

	return true;
}

void CWorld::UpdateFPS()
{
	CLOCK_TICKS_TIMESTAMP(ts);
	static int iterations = 0;

	++iterations;
	CLOCK_CURRENT_TICKS(ct);
	if (CLOCK_ELAPSED_TICKS(ts, ct) > FPS_UPDATE_INTERVAL)
	{
		m_fps = 1.0f / (CLOCK_ELAPSED_TICKS_IN_MS(ts, ct) / (float)iterations);
		iterations = 0;
		ts = ct;

		char final_msg[128];
		sprintf_s(final_msg, "FPS: %.2f", m_fps);

		char gameTime[64];
		sprintf_s(gameTime, " Game Time: %.2f", CLOCK->GetGameTimeInSeconds());
		strcat_s(final_msg, gameTime);

		char memUsed[32];
		sprintf_s(memUsed, " Memory Used: %.2fMB", MEM_ALLOCATOR->GetMemoryUsed() / (1024.0f * 1024.0f));
		strcat_s(final_msg, memUsed);

		if (m_isPaused)
			strcat_s(final_msg, " - Paused");

#ifdef USE_DXRENDERER
		SetWindowText(GOD::m_hWnd, final_msg);
#else
		LogInfo("%s", final_msg);
#endif
	}
}

bool CWorld::Update()
{
	UpdateFPS();

	if (m_inputManager) m_inputManager->ProcessInputStates();

	if (!m_isPaused)
	{
		CLOCK->UpdateGameTime();

		if (m_instanceManager) m_instanceManager->Update();
		
#ifdef USE_BOX2D
		if (GOD::m_physics) GOD::m_physics->Update();
#endif
	}

	return true;
}

void CWorld::Pause()
{
	m_isPaused = true;
}

void CWorld::Resume()
{
	m_isPaused = false;
}

void CWorld::TogglePause()
{
	m_isPaused ? Resume() : Pause();
}

void CWorld::CleanUp()
{
	SAFE_DELETE(m_inputManager);
	SAFE_DELETE(m_instanceManager);
}