#include "PhysicsModifier.h"

#ifdef USE_BOX2D

#include "Physics.h"
#include "PhysicsProxy.h"

#include "Game/World.h"
#include "Game/InstanceManager.h"

void CPhysicsModifier::Jump(float strength)
{
	b2Body* body = GOD::m_world->GetInstanceManager()->GetDynamicPhysicsProxy("BunnyDynamic")->GetBody();
	body->ApplyLinearImpulse(b2Vec2(0.0f, strength), body->GetWorldCenter(), true);
}

void CPhysicsModifier::Move(float x, float y)
{
	b2Body* body = GOD::m_world->GetInstanceManager()->GetDynamicPhysicsProxy("TestDynamic")->GetBody();
	body->ApplyLinearImpulse(b2Vec2(x, y), body->GetWorldCenter(), true);
}

void CPhysicsModifier::MoveBunny(float x, float y)
{
	b2Body* body = GOD::m_world->GetInstanceManager()->GetDynamicPhysicsProxy("BunnyDynamic")->GetBody();
	body->ApplyLinearImpulse(b2Vec2(x, y), body->GetWorldCenter(), true);
}

#endif