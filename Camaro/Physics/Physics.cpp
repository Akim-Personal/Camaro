#include "Physics.h"	

#ifdef USE_BOX2D

#ifdef USE_DXRENDERER
#include "PhysicsRenderer.h"
#endif

#include "PhysicsModifier.h"
#include <Game/World.h>
#include <Game/InstanceManager.h>

CPhysics::CPhysics()
	:
#ifdef USE_DXRENDERER
	m_physicsRenderer(NULL),
#endif
	m_physicsWorld(NULL),
	m_physicsModifier(NULL)
{}

bool CPhysics::Init()
{
	b2Vec2 gravity(0.0f, -9.81f);

	m_physicsWorld = new b2World(gravity);

	m_velocityIterations = 8;
	m_positionIterations = 3;

	m_physicsModifier = new CPhysicsModifier();

#ifdef USE_DXRENDERER
	m_physicsRenderer = new CPhysicsRenderer();

	uint flags =
		b2Draw::e_shapeBit +
		b2Draw::e_jointBit +
		b2Draw::e_aabbBit +
		b2Draw::e_pairBit +
		b2Draw::e_centerOfMassBit;

	m_physicsRenderer->SetFlags(flags);

	m_physicsWorld->SetDebugDraw(m_physicsRenderer);
#endif

	m_timeStep = 1.0f / 60.0f;

#ifdef USE_BOX2D
	if (GOD::m_world && !GOD::m_world->GetInstanceManager()->InitPhysics())
	{
		LogError("Could not initialize all Physics instances");
		return false;		
	}
#endif

	return true;
}

void CPhysics::Update()
{
	m_physicsWorld->Step(m_timeStep, m_velocityIterations, m_positionIterations);
	m_physicsWorld->Step(m_timeStep, m_velocityIterations, m_positionIterations);
	m_physicsWorld->Step(m_timeStep, m_velocityIterations, m_positionIterations);
}

void CPhysics::CleanUp()
{
#ifdef USE_DXRENDERER
	SAFE_DELETE(m_physicsRenderer);
#endif

	SAFE_DELETE(m_physicsModifier);
	SAFE_DELETE(m_physicsWorld);
}

#endif