#pragma once

#include <Common/Common.h>

#include "Memory/MemoryAllocator.h"
#include "Clock.h"
#include "Log.h"

class CWorld;
class CPhysics;
class CRenderer;

// The GOD class gives static access to the main modules. Every module itself is
// responsible to give further access to its members. The God.h needs be included
// in all headers of the game project to guarantee access to global project settings.
// GOD contains the main game update function which has to be called from the game
// loop every iteration.

class GOD
{
public:
	~GOD() { /* GOD contains static members only */ }

	static void CreateUniverse();
	static bool Progress();
	static void RequestShutdown() { m_shutdownRequested = true; }
	static void CleanUp();

	static CClock* m_clock;
	static CLog* m_log;
	static CMemoryAllocator* m_memoryAllocator;

	static CWorld* m_world;
	static CPhysics* m_physics;
	static CRenderer* m_renderer;

#ifdef USE_DXRENDERER
	static HINSTANCE m_hInst;
	static HWND m_hWnd;
#endif

private:
	GOD();
	static bool isAlive;
	static bool m_shutdownRequested;
};