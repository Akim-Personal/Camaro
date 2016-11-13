#include "Game/Instances/Solid.h"

#ifdef USE_BOX2D
#	include "Physics/PhysicsCommon.h"
#	include "Physics/PhysicsProxy.h"
#endif

#ifdef USE_DXRENDERER
#	include "Renderer/DX/DXRenderProxy.h"
#endif

CSolid::CSolid(char* name)
{
#ifdef USE_BOX2D
	m_physicsProxy = new CPhysicsProxy();
#endif

#ifdef USE_DXRENDERER
	m_renderProxy = new DXRenderProxy();
#endif

	m_name = name;
}

CSolid::~CSolid()
{
#ifdef USE_BOX2D
	SAFE_DELETE(m_physicsProxy);
#endif

#ifdef USE_DXRENDERER
	SAFE_DELETE(m_renderProxy);
#endif
}

bool CSolid::Init()
{
	// here we can initialise relevant graphics and sound
	return true;
}

#ifdef USE_BOX2D
bool CSolid::PreInitializePhysics(const void* props)
{
	m_properties = *((SSolidProperties*)props);

	return true;
}

bool CSolid::InitPhysics()
{
	SSolidProperties* p = &m_properties;

	b2BodyDef bodyDef;
	bodyDef.position.Set(p->posX, p->posY);
	bodyDef.userData = this;

	m_physicsProxy->CreateBody(&bodyDef);

	switch (p->type)
	{
	case e_Rectangle:
		{
			b2PolygonShape shape;
			shape.SetAsBox(p->boxWidth, p->boxHeight);
			m_physicsProxy->GetBody()->CreateFixture(&shape, 0.0f);
			return true;
		}
	case e_Circle:
		{
			b2CircleShape shape;
			shape.m_radius = p->radius;
			m_physicsProxy->GetBody()->CreateFixture(&shape, 0.0f);
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

void CSolid::Update()
{

}
