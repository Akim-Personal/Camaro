#include "PhysicsProxy.h"

#ifdef USE_BOX2D

#include "Physics.h"

CPhysicsProxy::CPhysicsProxy()
{
	m_physicsBody = NULL;
}

bool CPhysicsProxy::CreateBody(const b2BodyDef* bodyDef)
{
	m_physicsBody = GOD::m_physics->GetPhysicsWorld()->CreateBody(bodyDef);

	if (!m_physicsBody)
	{
		LogError("PhysicsBody could not be created");
		return false;
	}

	return true;
}

b2Fixture* CPhysicsProxy::CreateFixture(const b2FixtureDef* fixtureDef)
{
	if (m_physicsBody)
	{
		return m_physicsBody->CreateFixture(fixtureDef);
	}
	else
	{
		LogError("PhysicsBody was not initialized. Fixture has not been created");
	}

	return NULL;
}

#endif