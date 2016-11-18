#pragma once

#include <System/God.h>

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