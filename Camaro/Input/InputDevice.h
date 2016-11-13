#ifndef _IINPUTDEVICE_H_
#define _IINPUTDEVICE_H_

#include "System/God.h"

struct IInputDevice
{
	virtual ~IInputDevice() { ; }

	virtual bool Init() = 0;
	virtual void GetInputState() = 0;
	virtual void CleanUp() = 0;
};

#endif // _IINPUTDEVICE_H_