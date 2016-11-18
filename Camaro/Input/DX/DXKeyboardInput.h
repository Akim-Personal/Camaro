#pragma once

#include <Input/InputDevice.h>

#ifdef USE_DIRECTINPUT

class DXKeyboardInput : public IInputDevice
{
public:
	DXKeyboardInput();
	virtual ~DXKeyboardInput() { CleanUp(); }

	virtual bool Init();
	virtual void GetInputState();

private:
	LPDIRECTINPUT8				m_dxInput;
	LPDIRECTINPUTDEVICE8	m_dxKeyboardDevice;

	uint m_inputState;
	uint m_lastState;

	virtual void CleanUp();
};

#endif