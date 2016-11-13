#include "Game/Instances/Dynamic.h"

#ifdef USE_BOX2D
#	include "Physics/PhysicsCommon.h"
#	include "Physics/PhysicsProxy.h"
#endif

#ifdef USE_DXRENDERER
#	include "Renderer/DX/DXRenderProxy.h"
#endif

CDynamic::CDynamic(char* name)
{
#ifdef USE_BOX2D
	m_physicsProxy = new CPhysicsProxy();
#endif

#ifdef USE_DXRENDERER
	m_renderProxy = new DXRenderProxy();
#endif

	m_name = name;
}

CDynamic::~CDynamic()
{
#ifdef USE_BOX2D
	SAFE_DELETE(m_physicsProxy);
#endif

#ifdef USE_DXRENDERER
	SAFE_DELETE(m_renderProxy);
#endif
}

bool CDynamic::Init()
{
	// here we can initialise relevant graphics and sound
	return true;
}

#ifdef USE_BOX2D
bool CDynamic::PreInitializePhysics(const void* props)
{ 
	if (props)
		m_properties = *((SDynamicProperties*)props);

	return true;
}

bool CDynamic::InitPhysics()
{
	SDynamicProperties* p = &m_properties;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p->posX, p->posY);
	bodyDef.userData = this;

	m_physicsProxy->CreateBody(&bodyDef);

	switch (p->type)
	{
	case e_Rectangle:
		{
			b2PolygonShape shape;
			shape.SetAsBox(p->boxWidth, p->boxHeight);

			b2FixtureDef fixture;
			fixture.shape = &shape;
			fixture.restitution = 0.8f;
			fixture.density = 1.0f;
			fixture.friction = 0.8f;

			m_physicsProxy->GetBody()->CreateFixture(&fixture);
			return true;
		}
	case e_Circle:
		{
			b2CircleShape shape;
			shape.m_radius = p->radius;

			b2FixtureDef fixture;
			fixture.shape = &shape;
			fixture.restitution = 0.8f;
			fixture.density = 1.0f;
			fixture.friction = 0.8f;

			m_physicsProxy->GetBody()->CreateFixture(&fixture);
			return true;
		}
	case e_Polygon:
		{
			// not implemented
		}
	}

	return false;
}
#endif

void CDynamic::Update()
{

}