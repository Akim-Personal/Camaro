#pragma once

#include <System/God.h>

#ifdef USE_BOX2D

class b2Body;
struct b2BodyDef;
class b2Fixture;
struct b2FixtureDef;

class CPhysicsProxy
{
public:
	CPhysicsProxy();
	~CPhysicsProxy() { ; }

	bool CreateBody(const b2BodyDef* bodyDef);
	b2Body* GetBody() const { return m_physicsBody; }
	b2Fixture* CreateFixture(const b2FixtureDef* fixtureDef);

private:
	b2Body* m_physicsBody;
};

#endif