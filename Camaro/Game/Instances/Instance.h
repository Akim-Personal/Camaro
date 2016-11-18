#pragma once

#include <System/God.h>

#include "InstanceProperties.h"

#ifdef USE_BOX2D
class CPhysicsProxy;
#endif

#ifdef USE_DXRENDERER
class DXRenderProxy;
#endif

struct IInstance
{
	virtual ~IInstance() { ; }

	virtual bool Init() = 0;
#ifdef USE_BOX2D
	virtual bool PreInitializePhysics(const void* props) = 0;
	virtual bool InitPhysics() = 0;
#endif
	virtual void Update() = 0;

	virtual char* GetName() = 0;

#ifdef USE_BOX2D
	virtual CPhysicsProxy* GetPhysicsProxy() const = 0;
#endif

#ifdef USE_DXRENDERER
	virtual DXRenderProxy* GetRenderProxy() const = 0;
#endif

	virtual void GetSoundProxy() const = 0;
};