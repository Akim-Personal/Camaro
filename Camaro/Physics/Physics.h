#pragma once

#include <System/God.h>

#ifdef USE_BOX2D

#include "PhysicsCommon.h"

class CPhysicsModifier;
class CPhysicsRenderer;

class CPhysics
{
public:
	CPhysics();
	~CPhysics() { CleanUp(); }

	bool Init();
	void Update();

	CPhysicsModifier* GetPhysicsModifier() const { return m_physicsModifier; }
	b2World* GetPhysicsWorld() { return m_physicsWorld; }

#ifdef USE_DXRENDERER
	CPhysicsRenderer* GetPhysicsRenderer() const { return m_physicsRenderer; }
#endif

private:
	int m_velocityIterations;
	int m_positionIterations;

	b2World* m_physicsWorld;

	void CleanUp();

#ifdef USE_DXRENDERER
	CPhysicsRenderer* m_physicsRenderer;
#endif

	CPhysicsModifier* m_physicsModifier;

	float m_timeStep;
};

#endif