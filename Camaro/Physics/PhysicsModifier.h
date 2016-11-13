#ifndef _CPHYSICSMODIFIER_H_
#define _CPHYSICSMODIFIER_H_

#include "System/God.h"

#ifdef USE_BOX2D

class CPhysicsModifier
{
public:
	CPhysicsModifier() { ; }
	~CPhysicsModifier() { ; }
	
	void Jump(float strength);
	void Move(float x, float y);
	void MoveBunny(float x, float y);
};

#endif

#endif // _CPHYSICSMODIFIER_H_
