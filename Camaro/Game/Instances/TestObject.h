#ifndef _CTESTOBJECT_H_
#define _CTESTOBJECT_H_

#include "Game/Instances/Dynamic.h"

class CTestObject : public CDynamic
{
public:
	CTestObject(char* name);
	virtual ~CTestObject();

	virtual bool Init();
#ifdef USE_BOX2D
	virtual bool PreInitializePhysics(const void* props);
	virtual bool InitPhysics();
#endif

	virtual void Update();

private:
	CTestObject() { ; }

#ifdef USE_BOX2D
	CPhysicsProxy* m_physicsProxy2;
	CPhysicsProxy* m_physicsProxy3;
#endif
};

#endif // _CTESTOBJECT_H_