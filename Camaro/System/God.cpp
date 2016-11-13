#include "God.h"

#include "Game/World.h"
#ifdef USE_BOX2D
#	include "Physics/Physics.h"
#endif
#include "Renderer/Renderer.h"

CClock* GOD::m_clock = NULL;
CLog* GOD::m_log = NULL;
CMemoryAllocator* GOD::m_memoryAllocator = NULL;

CWorld* GOD::m_world = NULL;
#ifdef USE_BOX2D
CPhysics* GOD::m_physics = NULL;
#endif
#ifdef USE_DXRENDERER
HINSTANCE GOD::m_hInst = NULL;
HWND GOD::m_hWnd = NULL;
#endif

bool GOD::isAlive = false;
bool GOD::m_shutdownRequested = false;

CRenderer* GOD::m_renderer = NULL;

void GOD::CreateUniverse()
{
	if (!isAlive)
	{
		m_memoryAllocator = new CMemoryAllocator();	// start tracking of memory leaks
		m_clock = new CClock();
		m_log = new CLog();

#ifdef USE_DXRENDERER
		ASSERT(m_hInst);
		ASSERT(m_hWnd);
#endif

		m_world = new CWorld();
		if (m_world && !m_world->Init())
			LogFatalError("World could not be initialized");

#ifdef USE_BOX2D
		m_physics = new CPhysics();
		if (m_physics && !m_physics->Init())
			LogFatalError("Physics could not be initialized");
#else
		LogInfo("Box2D has been excluded from Build and was not initialized");
#endif

		m_renderer = new CRenderer();
		if (m_renderer && !m_renderer->Init())
			LogFatalError("Renderer could not be initialized");

		isAlive = true;
	}
}

bool GOD::Progress()
{
	if (isAlive)
	{
		if (m_world) m_world->Update();
		if (m_renderer) m_renderer->Render();

		if (m_shutdownRequested)
		{
			LogInfo("Shutdown has been requested by user");
			return false;
		}
		return true;
	}

	return false;
}

void GOD::CleanUp()
{
	if (isAlive)
	{
		SAFE_DELETE(m_renderer);
#ifdef USE_BOX2D
		SAFE_DELETE(m_physics);
#endif
		SAFE_DELETE(m_world);

		LogInfo("God has successfully released resources. Going to shutdown remaining system helpers ...");
		SAFE_DELETE(m_log);
		SAFE_DELETE(m_clock);
		SAFE_DELETE(m_memoryAllocator);
	}
}
