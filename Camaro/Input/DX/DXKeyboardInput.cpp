#include "DXKeyboardInput.h"

#ifdef USE_DIRECTINPUT

#include "Renderer/Renderer.h"
#include "Game/World.h"
#include "../InputManager.h"

DXKeyboardInput::DXKeyboardInput()
	: IInputDevice()
{
	m_dxInput = NULL;
	m_dxKeyboardDevice = NULL;

	m_inputState = 0;
	m_lastState = 0;
}

bool DXKeyboardInput::Init()
{
	HRESULT hr;

	hr = DirectInput8Create(GOD::m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_dxInput, NULL); 

	if FAILED(hr)
	{
		LogError("Keyboard: Failed to create Input");
		return false; 
	}

	hr = m_dxInput->CreateDevice(GUID_SysKeyboard, &m_dxKeyboardDevice, NULL); 

	if FAILED(hr) 
	{ 
		LogError("Keyboard: Failed to create Device");
		CleanUp();
		return false; 
	} 

	hr = m_dxKeyboardDevice->SetDataFormat(&c_dfDIKeyboard); 
	if FAILED(hr) 
	{ 
		LogError("Keyboard: Failed to set Data Format");
		CleanUp();
		return false; 
	} 

	hr = m_dxKeyboardDevice->SetCooperativeLevel(GOD::m_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
	if FAILED(hr) 
	{ 
		LogError("Keyboard: Failed to set Cooperative Level");
		CleanUp(); 
		return false; 
	}

	m_dxKeyboardDevice->Acquire();

	return true; 
}

void DXKeyboardInput::GetInputState()
{ 
#define KEYDOWN(name, key) (name[key] & 0x80) 

	char     buffer[256];
	HRESULT  hr;

	ZeroMemory(&buffer, 256);

	hr = m_dxKeyboardDevice->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 
	if FAILED(hr) 
	{ 
		hr = m_dxKeyboardDevice->Acquire();
		while (hr == DIERR_INPUTLOST) hr = m_dxKeyboardDevice->Acquire();

		hr = m_dxKeyboardDevice->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 
	} 

	uint m_beforeLastState = m_lastState;
	m_lastState = m_inputState;
	m_inputState = 0;

	if (KEYDOWN(buffer, DIK_UP)) m_inputState |= e_Key_Up;
	if (KEYDOWN(buffer, DIK_DOWN)) m_inputState |= e_Key_Down;
	if (KEYDOWN(buffer, DIK_LEFT)) m_inputState |= e_Key_Left;
	if (KEYDOWN(buffer, DIK_RIGHT))	m_inputState |= e_Key_Right;
	if (KEYDOWN(buffer, DIK_SPACE))	m_inputState |= e_Key_Space;
	if (KEYDOWN(buffer, DIK_ESCAPE)) m_inputState |= e_Key_ESC;
	if (KEYDOWN(buffer, DIK_P)) m_inputState |= e_Key_P;

	if ((m_beforeLastState != m_lastState) || (m_inputState != m_lastState))
		GOD::m_world->GetInputManager()->SetKeyboardInputState(m_inputState, m_lastState);
}

void DXKeyboardInput::CleanUp() 
{ 
	if (m_dxInput) 
	{ 
		if (m_dxKeyboardDevice) 
		{ 
			m_dxKeyboardDevice->Unacquire(); 
			m_dxKeyboardDevice->Release();
			m_dxKeyboardDevice = NULL; 
		} 
		m_dxInput->Release();
		m_dxInput = NULL; 
	}
} 

#endif