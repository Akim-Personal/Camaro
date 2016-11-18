#include "DXGamepadInput.h"

#ifdef USE_XINPUT
#include <Renderer/Renderer.h>
#include <Game/World.h>
#include <Input/InputManager.h>

#pragma comment(lib, "XInput.lib")

#define TRIGGER_RANGE 0xFF
#define THUMBSTICK_RANGE 0x7FFF
#define RUMBLE_RANGE 0xFFFF

#define CONNECTIVITY_CHECK_INTERVAL CLOCK_TICKS_AS_MS(2000)

DXGamepadInput::DXGamepadInput()
	: IInputDevice()
{
	for (int i = 0; i < MAX_CONTROLLERS; ++i)
	{
		// TODO: CHECK HERE IF ANY OTHER MEMBERS OF STRUCT NEED TO BE INITIALISED.
		//WOULDNT IT BE BETTER TO HAVE THEM INITIALISED IN THE CONSTRUCTOR OF THE STRUCT????
		m_Controllers[i].bConnected = false;
		m_Controllers[i].bLockVibration = false;
		m_Controllers[i].vibration.wLeftMotorSpeed = 0;
		m_Controllers[i].vibration.wRightMotorSpeed = 0;
		m_Controllers[i].vibration_left_total = 0;
		m_Controllers[i].vibration_right_total = 0;
		m_Controllers[i].vibration_lastTimestamp = 0;
		m_Controllers[i].normalized_thumb_lx = 0.0f;
		m_Controllers[i].normalized_thumb_ly = 0.0f;
		m_Controllers[i].magnitude_left_thumb = 0.0f;
		m_Controllers[i].normalized_thumb_rx = 0.0f;
		m_Controllers[i].normalized_thumb_ry = 0.0f;
		m_Controllers[i].magnitude_right_thumb = 0.0f;
		m_Controllers[i].normalized_trigger_lt = 0.0f;
		m_Controllers[i].normalized_trigger_rt = 0.0f;
		m_Controllers[i].timeAtLastConnectivityCheck = -CONNECTIVITY_CHECK_INTERVAL;
	}

	XInputEnable(true);
}

bool DXGamepadInput::Init()
{
	return true;
}

void DXGamepadInput::GetInputState()
{ 
	DWORD dwResult;
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		CONTROLLER_STATE* pad = &(m_Controllers[i]);

		pad->beforeLastState = pad->lastState;
		pad->lastState = pad->state;

		CLOCK_CURRENT_TICKS(ct);
		if ((pad->bConnected) || (CLOCK_ELAPSED_TICKS(pad->timeAtLastConnectivityCheck, ct) > CONNECTIVITY_CHECK_INTERVAL))
		{
			dwResult = XInputGetState(i, &m_Controllers[i].state);

			if (dwResult == ERROR_SUCCESS)
			{
				pad->bConnected = true;

				if (pad->lastState.dwPacketNumber != m_Controllers[i].state.dwPacketNumber)
					NormalizeAnalogueInput(i);
			}
			else
			{
				pad->bConnected = false;
				m_Controllers[i].timeAtLastConnectivityCheck = ct;
			}
		}
	}
	
	for (int i = 0; i < MAX_CONTROLLERS; ++i)
	{
		CONTROLLER_STATE* pad = &(m_Controllers[i]);

		if ((pad->beforeLastState.dwPacketNumber != pad->lastState.dwPacketNumber)
			|| (pad->state.dwPacketNumber != pad->lastState.dwPacketNumber))
		{
			GOD::m_world->GetInputManager()->SetGamepadInputState(m_Controllers, MAX_CONTROLLERS);
			break;
		}
	}

	UpdateRumbleStates();
}

void DXGamepadInput::NormalizeThumbStick(int sThumbX, int sThumbY,
	float* normalizedMagnitude, float* normalizedX, float* normalizedY, uint thumb_deadzone)
{
	float x = (float)sThumbX;
	float y = (float)sThumbY;

	float magnitude = sqrt(x*x + y*y);

	if (magnitude > thumb_deadzone)
	{
		if (magnitude > THUMBSTICK_RANGE) magnitude = THUMBSTICK_RANGE;
		magnitude -= thumb_deadzone;

		*normalizedMagnitude = magnitude / (float)(THUMBSTICK_RANGE - thumb_deadzone);

		float multiplier = 1.0f;
		if ((float)sThumbX < 0.0f)
			multiplier = -1.0f;

		if ((multiplier * sThumbX) > thumb_deadzone)
		{
			*normalizedX = multiplier * 
				((float)(multiplier * sThumbX - thumb_deadzone) /
				(float)(THUMBSTICK_RANGE - thumb_deadzone));
		}
		else
			*normalizedX = 0.0f;

		multiplier = 1.0f;
		if ((float)sThumbY < 0.0f)
			multiplier = -1.0f;

		if ((multiplier * sThumbY) > thumb_deadzone)
		{
			*normalizedY = multiplier * 
				((float)(multiplier * sThumbY - thumb_deadzone) /
				(float)(THUMBSTICK_RANGE - thumb_deadzone));
		}
		else
			*normalizedY = 0.0f;
	}
	else
	{
		*normalizedMagnitude = 0.0f;
		*normalizedX = 0.0f;
		*normalizedY = 0.0f;
	}
}

void DXGamepadInput::NormalizeAnalogueInput(int id)
{
	XINPUT_GAMEPAD* pad = &(m_Controllers[id].state.Gamepad);

	// Normalize left trigger
	if (pad->bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		m_Controllers[id].normalized_trigger_lt =
			(float)(pad->bLeftTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			/ (float)(TRIGGER_RANGE - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}
	else
		m_Controllers[id].normalized_trigger_lt = 0.0f;

	// Normalize right trigger
	if (pad->bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		m_Controllers[id].normalized_trigger_rt =
			(float)(pad->bRightTrigger - XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
			/ (float)(TRIGGER_RANGE - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}
	else
		m_Controllers[id].normalized_trigger_rt = 0.0f;

	// Normalize left thumb stick
	NormalizeThumbStick(pad->sThumbLX, pad->sThumbLY, &m_Controllers[id].magnitude_left_thumb,
		&m_Controllers[id].normalized_thumb_lx, &m_Controllers[id].normalized_thumb_ly, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

	// Normalize right thumb stick
	NormalizeThumbStick(pad->sThumbRX, pad->sThumbRY, &m_Controllers[id].magnitude_right_thumb,
		&m_Controllers[id].normalized_thumb_rx, &m_Controllers[id].normalized_thumb_ry, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
}

void DXGamepadInput::SetRumbleStrength(int id, bool highFrequence, uint strength, bool substract)
{
	int multiplier;
	multiplier = substract ? -1 : 1;

	WORD* rumbler = NULL;
	int* totalStrength = NULL;
	if (highFrequence)
	{
		rumbler = &(m_Controllers[id].vibration.wRightMotorSpeed);
		totalStrength = &(m_Controllers[id].vibration_right_total);
	}
	else
	{
		rumbler = &(m_Controllers[id].vibration.wLeftMotorSpeed);
		totalStrength = &(m_Controllers[id].vibration_left_total);
	}

	*totalStrength += strength * multiplier;

	if (*totalStrength > RUMBLE_RANGE)
		*rumbler = RUMBLE_RANGE;
	else
		*rumbler = (WORD)(*totalStrength);
}

void DXGamepadInput::Rumble(int id, bool highFrequence, float strength, clock_t duration)
{
	int convertedStrength = (int)(strength * RUMBLE_RANGE);
	if (convertedStrength > RUMBLE_RANGE)
		convertedStrength = RUMBLE_RANGE;
	else if (convertedStrength < 0.0f)
		convertedStrength = 0;

	ASSERT(convertedStrength >= 0);
	SetRumbleStrength(id, highFrequence, convertedStrength, false);

	XInputSetState(id, &(m_Controllers[id].vibration));

	SRumble newEntry;
	newEntry.strength = (WORD)convertedStrength;
	newEntry.lifetime = duration;
	newEntry.highFrequence = highFrequence;

	m_Controllers[id].vibration_list.push_back(newEntry);
}

void DXGamepadInput::UpdateRumbleStates()
{
	for (int i = 0; i < MAX_CONTROLLERS; i++)
	{
		CONTROLLER_STATE* pad = &m_Controllers[i];
		bool updateRequired = false;

		if (pad->bConnected)
		{
			CLOCK_CURRENT_TICKS(ct);

			for (std::list<SRumble>::iterator it = pad->vibration_list.begin(); it != pad->vibration_list.end(); )
			{
				it->lifetime -= CLOCK_ELAPSED_TICKS(pad->vibration_lastTimestamp, ct);

				if (it->lifetime < 0)
				{
					SetRumbleStrength(i, it->highFrequence, it->strength, true);
					
					it = pad->vibration_list.erase(it);
					updateRequired = true;
				}
				else
				{
					++it;
				}
			}
			pad->vibration_lastTimestamp = ct;
		}
		
		if (updateRequired)
			XInputSetState(i, &(pad->vibration));
	}
}

void DXGamepadInput::CleanUp() 
{ 
	XInputEnable(false);
} 

#endif