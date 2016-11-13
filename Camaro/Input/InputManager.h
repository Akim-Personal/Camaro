#ifndef _CINPUTMANAGER_H_
#define _CINPUTMANAGER_H_

#include "System/God.h"

struct CONTROLLER_STATE;

#ifdef USE_DIRECTINPUT
class DXKeyboardInput;
#endif

#ifdef USE_XINPUT
class DXGamepadInput;
#endif

enum EKeys
{	
	e_Key_Down		  = 0x01,
	e_Key_Up				= 0x02,
	e_Key_Left			= 0x04,
	e_Key_Right			= 0x08,
	e_Key_Space			= 0x10,
	e_Key_ESC				= 0x20,
	e_Key_P					= 0x40
};

class CInputManager
{
public:
	CInputManager();
	~CInputManager();

#ifdef USE_DIRECTINPUT
	DXKeyboardInput* GetKeyboardInput() const { return m_DXKeyboardInput; }
	void SetKeyboardInputState(uint inputState, uint lastState);
#endif

#ifdef USE_XINPUT
	DXGamepadInput* GetGamepadInput() const { return m_DXGamepadInput; }
void SetGamepadInputState(const CONTROLLER_STATE* inputState, int count);
#endif

	void ProcessInputStates();

private:
	struct KEYBOARD_STATE
	{
		uint lastState;
		uint state;
	};

	void InterpretInputStates();

#ifdef USE_DIRECTINPUT
	DXKeyboardInput* m_DXKeyboardInput;
	KEYBOARD_STATE m_keyboardInputState;
#endif

#ifdef USE_XINPUT
	DXGamepadInput* m_DXGamepadInput;
	CONTROLLER_STATE* m_gamepadInputState;
	int m_gamepadCount;
#endif
};

#endif // _CINPUTMANAGER_H_