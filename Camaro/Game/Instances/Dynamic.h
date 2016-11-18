#pragma once

#include "Instance.h"

class CDynamic : public IInstance
{
public:
	CDynamic(char* name);
	virtual ~CDynamic();

	virtual bool Init();
#ifdef USE_BOX2D
	virtual bool PreInitializePhysics(const void* props);
	virtual bool InitPhysics();
#endif

	virtual void Update();

	virtual char* GetName() { return m_name; }

#ifdef USE_BOX2D
	virtual CPhysicsProxy* GetPhysicsProxy() const { return m_physicsProxy; }
#endif

#ifdef USE_DXRENDERER
	virtual DXRenderProxy* GetRenderProxy() const { return m_renderProxy; }
#endif

	virtual void GetSoundProxy() const { ; }

protected:
	CDynamic() { ; }

	char* m_name;
#ifdef USE_BOX2D
	CPhysicsProxy* m_physicsProxy;
	SDynamicProperties m_properties;
#endif

#ifdef USE_DXRENDERER
	DXRenderProxy* m_renderProxy;
#endif
};