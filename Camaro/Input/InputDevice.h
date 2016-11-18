#pragma once

#include <System/God.h>

struct IInputDevice
{
	virtual ~IInputDevice() { ; }

	virtual bool Init() = 0;
	virtual void GetInputState() = 0;
	virtual void CleanUp() = 0;
};