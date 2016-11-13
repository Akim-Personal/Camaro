#ifndef _CSOLID_H_
#define _CSOLID_H_

#include "Game/Instances/Instance.h"

class CSolid : public IInstance
{
public:
	CSolid(char* name);
	virtual ~CSolid();

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
	CSolid() { ; }

	char* m_name;
#ifdef USE_BOX2D
	CPhysicsProxy* m_physicsProxy;
	SSolidProperties m_properties;
#endif

#ifdef USE_DXRENDERER
	DXRenderProxy* m_renderProxy;
#endif
};

#endif // _CSOLID_H_