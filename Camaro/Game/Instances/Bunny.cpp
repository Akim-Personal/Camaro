#include "Bunny.h"

#ifdef USE_BOX2D
#	include <Physics/Physics.h>
#	include <Physics/PhysicsProxy.h>
#endif

CBunny::CBunny(char* name)
	:CDynamic(name)
{}

bool CBunny::Init()
{
	return CDynamic::Init();
}

#ifdef USE_BOX2D
bool CBunny::PreInitializePhysics(const void* props)
{
	return CDynamic::PreInitializePhysics(props);
}

bool CBunny::InitPhysics()
{
	b2World* pWorld = GOD::m_physics->GetPhysicsWorld();

	{
		// Head
		b2CircleShape shape;
		shape.m_radius = 1.5f;

		b2FixtureDef fixture;
		fixture.shape = &shape;
		fixture.density = 1.0f;
		fixture.restitution = 0.1f;
		fixture.friction = 0.5f;

		b2BodyDef bodyDef;
		//bodyDef.gravityScale = 0.5f;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(-5.0f, 1.0f);
		bodyDef.fixedRotation = true;
		m_physicsProxy->CreateBody(&bodyDef);
		m_physicsProxy->CreateFixture(&fixture);
	}

	{
		b2PolygonShape shape;
		shape.SetAsBox(0.25f, 0.25f, b2Vec2(0.0f, 0.0f), 0.0f);

		b2FixtureDef fixture;
		fixture.shape = &shape;
		fixture.density = 1.01f;
		//fixture.filter.maskBits = 0x0000;

		b2WeldJointDef jd;

		b2Body* prevBody;

		// right ear
		prevBody = m_physicsProxy->GetBody();
		for (int i = 0; i < 4; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.linearDamping = 0.9f;
			bd.position.Set(-4.0f + 0.5f * i, 3.0f + 0.5f * i);
			b2Body* body = pWorld->CreateBody(&bd);
			body->CreateFixture(&fixture);

			b2Vec2 anchor(-4.25f + 0.5f * i, 3.0f + 0.5f * i);
			jd.Initialize(prevBody, body, anchor);
			pWorld->CreateJoint(&jd);

			prevBody = body;
		}

		// left ear
		prevBody = m_physicsProxy->GetBody();
		for (int i = 0; i < 4; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.linearDamping = 0.9f;
			bd.position.Set(-6.0f - 0.5f * i, 3.0f + 0.5f * i);
			b2Body* body = pWorld->CreateBody(&bd);
			body->CreateFixture(&fixture);

			b2Vec2 anchor(-5.75f - 0.5f * i, 3.0f + 0.5f * i);
			jd.Initialize(prevBody, body, anchor);
			pWorld->CreateJoint(&jd);

			prevBody = body;
		}
	}

	return true;
}
#endif

void CBunny::Update()
{
	CDynamic::Update();
}