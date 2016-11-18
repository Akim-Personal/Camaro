#pragma once

#include <System/God.h>

class CInputManager;
class CInstanceManager;

class CWorld
{
public:
	CWorld();
	~CWorld() { CleanUp(); }

	bool Init();
	void Update();

	void Pause();
	void Resume();
	void TogglePause();
	bool IsPaused() const { return m_isPaused; }

	float GetFPS() const { return m_fps; }

	CInputManager* GetInputManager() const { return m_inputManager; }
	CInstanceManager* GetInstanceManager() const { return m_instanceManager; }

private:
	void CleanUp();
	void UpdateFPS();

	CInputManager* m_inputManager;
	CInstanceManager* m_instanceManager;

	float m_fps;
	bool m_isPaused;
};