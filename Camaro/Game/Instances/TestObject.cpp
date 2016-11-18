#include "TestObject.h"

#ifdef USE_BOX2D
#	include <Physics/Physics.h>
#	include <Physics/PhysicsProxy.h>
#endif

CTestObject::CTestObject(char* name)
	:CDynamic(name)
{
#ifdef USE_BOX2D
	m_physicsProxy2 = new CPhysicsProxy();
	m_physicsProxy3 = new CPhysicsProxy();
#endif
}

CTestObject::~CTestObject()
{
#ifdef USE_BOX2D
	SAFE_DELETE(m_physicsProxy2);
	SAFE_DELETE(m_physicsProxy3);
#endif
}

bool CTestObject::Init()
{
	return CDynamic::Init();
}

#ifdef USE_BOX2D
bool CTestObject::PreInitializePhysics(const void* props)
{
	return CDynamic::PreInitializePhysics(props);
}

bool CTestObject::InitPhysics()
{
	b2World* pWorld = GOD::m_physics->GetPhysicsWorld();

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 5.0);
	m_physicsProxy->CreateBody(&bodyDef);

	b2BodyDef bodyDef3;
	bodyDef3.type = b2_dynamicBody;
	bodyDef3.position.Set(15.0f, 15.0);
	m_physicsProxy3->CreateBody(&bodyDef3);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.restitution = 0.0f;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	m_physicsProxy->CreateFixture(&fixtureDef);
	m_physicsProxy3->CreateFixture(&fixtureDef);


	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef2;
	bodyDef2.type = b2_dynamicBody;
	bodyDef2.position.Set(1.0f, 20.0f);
	m_physicsProxy2->CreateBody(&bodyDef2);

	// Define a circle shape for our dynamic body.
	b2CircleShape dynamicCircle;
	dynamicCircle.m_p.Set(0.0f, 0.0f);
	dynamicCircle.m_radius = 2.0f;

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef2;
	fixtureDef2.shape = &dynamicCircle;
	fixtureDef2.restitution = 0.8f;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef2.density = 1.0f;

	// Override the default friction.
	fixtureDef2.friction = 0.8f;

	// Add the shape to the body.
	m_physicsProxy2->CreateFixture(&fixtureDef2);

	b2DistanceJointDef jointDef;
	jointDef.Initialize(m_physicsProxy->GetBody(), m_physicsProxy3->GetBody(),
		m_physicsProxy->GetBody()->GetWorldCenter(), m_physicsProxy3->GetBody()->GetWorldCenter());
	jointDef.collideConnected = true;
	pWorld->CreateJoint(&jointDef);

	return true;
}
#endif

void CTestObject::Update()
{
	CDynamic::Update();
}