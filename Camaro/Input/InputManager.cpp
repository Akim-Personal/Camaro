#include "InputManager.h"

#ifdef USE_BOX2D
#	include <Physics/Physics.h>
#	include <Physics/PhysicsModifier.h>
#endif

#include <Game/World.h>

#include <Renderer/Renderer.h>

#ifdef USE_DIRECTINPUT
#include "DX/DXKeyboardInput.h"
#endif

#ifdef USE_XINPUT
#include "DX/DXGamepadInput.h"
#endif

#ifdef USE_DIRECTINPUT
#	define KEY_DOWN(inputState, key) ((inputState.state & key) && (!(inputState.lastState & key)))
#	define KEY_PRESSED(inputState, key) (inputState.state & key)
#	define KEY_UP(inputState, key) ((inputState.lastState & key) && (!(inputState.state & key)))
#endif

#ifdef USE_XINPUT
#	define BUTTON_DOWN(inputState, key) ((inputState.state.Gamepad.wButtons & key) && (!(inputState.lastState.Gamepad.wButtons & key)))
#	define BUTTON_PRESSED(inputState, key) (inputState.state.Gamepad.wButtons & key)
#	define BUTTON_UP(inputState, key) ((inputState.lastState.Gamepad.wButtons & key) && (!(inputState.state.Gamepad.wButtons & key)))

#	define THUMB_LX(inputState, key) (inputState.state.Gamepad.sThumbLX)
#	define THUMB_LY(inputState, key) (inputState.state.Gamepad.sThumbLY)
#	define THUMB_RX(inputState, key) (inputState.state.Gamepad.sThumbRX)
#	define THUMB_RY(inputState, key) (inputState.state.Gamepad.sThumbRY)
#	define TRIGGER_LT(inputState, key) (inputState.state.Gamepad.bLeftTrigger)
#	define TRIGGER_RT(inputState, key) (inputState.state.Gamepad.bRightTrigger)
#endif

CInputManager::CInputManager()
{
#ifdef USE_DIRECTINPUT
	m_DXKeyboardInput = new DXKeyboardInput();
	if (m_DXKeyboardInput && !m_DXKeyboardInput->Init())
			LogFatalError("DXKeyboardInput could not be initialized");
#endif

#ifdef USE_XINPUT
	m_gamepadInputState = NULL;
	m_gamepadCount = 0;

	m_DXGamepadInput = new DXGamepadInput();
	if (m_DXGamepadInput && !m_DXGamepadInput->Init())
			LogFatalError("DXGamepadInput could not be initialized");
#endif
}

CInputManager::~CInputManager()
{
#ifdef USE_XINPUT
	SAFE_DELETE(m_DXGamepadInput);
	SAFE_DELETE_ARRAY(m_gamepadInputState);
#endif

#ifdef USE_DIRECTINPUT
	SAFE_DELETE(m_DXKeyboardInput);
#endif
}

#ifdef USE_DIRECTINPUT
void CInputManager::SetKeyboardInputState(uint inputState, uint lastState)
{
	m_keyboardInputState.state = inputState;
	m_keyboardInputState.lastState = lastState;
}
#endif

#ifdef USE_XINPUT
void CInputManager::SetGamepadInputState(const CONTROLLER_STATE* inputState, int count)
{
	if (!m_gamepadInputState)
		m_gamepadInputState = new CONTROLLER_STATE[count];

	for (int i = 0; i < count; ++i)
	{
		m_gamepadInputState[i] = inputState[i];
		m_gamepadCount = count;
	}
}
#endif

void CInputManager::ProcessInputStates()
{
#ifdef USE_DIRECTINPUT
	if (m_DXKeyboardInput) m_DXKeyboardInput->GetInputState();
#endif

#ifdef USE_XINPUT
	if (m_DXGamepadInput) m_DXGamepadInput->GetInputState();
#endif

	InterpretInputStates();
}

void CInputManager::InterpretInputStates()
{
	if (GOD::m_world->IsPaused())
	{
#ifdef USE_DIRECTINPUT
		if (KEY_UP(m_keyboardInputState, e_Key_ESC))
		{
			GOD::RequestShutdown();
		}
		if (KEY_DOWN(m_keyboardInputState, e_Key_P))
		{
			CLOCK->ResetGameTime();
			GOD::m_world->TogglePause();
			return;
		}
#endif
	}
	else
	{
#ifdef USE_BOX2D
		CPhysicsModifier* physicsModifier = GOD::m_physics->GetPhysicsModifier();
#endif

#ifdef USE_DXRENDERER
		CRenderer* renderer = GOD::m_renderer;
#endif

#ifdef USE_DIRECTINPUT
		if (KEY_DOWN(m_keyboardInputState, e_Key_Space))
		{
#ifdef USE_BOX2D
			physicsModifier->Jump(160.0f);
#endif
		}
		if (KEY_PRESSED(m_keyboardInputState, e_Key_Right))
		{
#ifdef USE_DXRENDERER
			renderer->SetCameraPosition(renderer->GetCameraPosition() + D3DXVECTOR3(0.3f, 0.0f, 0.0f));
#endif
		}
		if (KEY_PRESSED(m_keyboardInputState, e_Key_Left))
		{
#ifdef USE_DXRENDERER
			renderer->SetCameraPosition(renderer->GetCameraPosition() + D3DXVECTOR3(-0.3f, 0.0f, 0.0f));
#endif
		}
		if (KEY_UP(m_keyboardInputState, e_Key_ESC))
		{
			GOD::RequestShutdown();
		}
		if (KEY_DOWN(m_keyboardInputState, e_Key_P))
		{
			GOD::m_world->TogglePause();
		}
#endif

#ifdef USE_XINPUT
		for (int i = 0; i < m_gamepadCount; ++i)
		{
			if (m_gamepadInputState && m_gamepadInputState[i].bConnected)
			{
				if (BUTTON_UP(m_gamepadInputState[i], XINPUT_GAMEPAD_BACK))
				{
					GOD::RequestShutdown();
				}

				if (BUTTON_DOWN(m_gamepadInputState[i], XINPUT_GAMEPAD_A))
				{
#ifdef USE_BOX2D
					physicsModifier->Jump(160.0f);
#endif
					m_DXGamepadInput->Rumble(i, true, 0.2f, CLOCK_TICKS_AS_MS(300));
					m_DXGamepadInput->Rumble(i, false, 0.2f, CLOCK_TICKS_AS_MS(300));
				}

				if ((BUTTON_DOWN(m_gamepadInputState[i], XINPUT_GAMEPAD_DPAD_LEFT))
					|| BUTTON_PRESSED(m_gamepadInputState[i], XINPUT_GAMEPAD_DPAD_LEFT))
				{
#ifdef USE_BOX2D
					physicsModifier->MoveBunny(-3.0f, 0.0f);
#endif
				}

				if ((BUTTON_DOWN(m_gamepadInputState[i], XINPUT_GAMEPAD_DPAD_RIGHT))
					|| (BUTTON_PRESSED(m_gamepadInputState[i], XINPUT_GAMEPAD_DPAD_RIGHT)))
				{
#ifdef USE_BOX2D
					physicsModifier->MoveBunny(3.0f, 0.0f);
#endif
				}

				if (m_gamepadInputState[i].normalized_thumb_lx != 0 || m_gamepadInputState[i].normalized_thumb_ly != 0)
				{
#ifdef USE_BOX2D
					physicsModifier->Move(2.0f * m_gamepadInputState[i].normalized_thumb_lx, 2.0f * m_gamepadInputState[i].normalized_thumb_ly);
#endif
				}

#ifdef USE_DXRENDERER
				if (m_gamepadInputState[i].normalized_thumb_rx != 0 || m_gamepadInputState[i].normalized_thumb_ry != 0)
					renderer->SetCameraPosition(renderer->GetCameraPosition() + D3DXVECTOR3(m_gamepadInputState[i].normalized_thumb_rx, m_gamepadInputState[i].normalized_thumb_ry, 0.0f));

				if (m_gamepadInputState[i].normalized_trigger_lt != 0)
					renderer->SetCameraPosition(renderer->GetCameraPosition() + D3DXVECTOR3(0.0f, 0.0f, -m_gamepadInputState[i].normalized_trigger_lt));

				if (m_gamepadInputState[i].normalized_trigger_rt != 0)
					renderer->SetCameraPosition(renderer->GetCameraPosition() + D3DXVECTOR3(0.0f, 0.0f, m_gamepadInputState[i].normalized_trigger_rt));
#endif
			}
		}
#endif
	}
}