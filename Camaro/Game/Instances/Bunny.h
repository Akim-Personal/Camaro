#pragma once

#include "Dynamic.h"

class CBunny : public CDynamic
{
public:
	CBunny(char* name);
	virtual ~CBunny() { ; }

	virtual bool Init();
#ifdef USE_BOX2D
	virtual bool PreInitializePhysics(const void* props);
	virtual bool InitPhysics();
#endif

	virtual void Update();

private:
	CBunny() { ; }
};