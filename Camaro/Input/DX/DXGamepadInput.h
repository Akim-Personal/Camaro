#ifndef _DXGAMEPADINPUT_H_
#define _DXGAMEPADINPUT_H_

#include "Input/InputDevice.h"

#ifdef USE_XINPUT

#define MAX_CONTROLLERS 4

struct SRumble
{
	WORD strength;
	clock_t lifetime;
	bool highFrequence;
};

struct CONTROLLER_STATE
{
	// TODO: ADD CONSTRUCTOR WITH INITIALIZERS HERE INSTEAD OF CONSTRUCTOR OF INPUT CLASS
	XINPUT_STATE beforeLastState;
	XINPUT_STATE lastState;
	XINPUT_STATE state;
	bool bConnected;
	bool bLockVibration;
	XINPUT_VIBRATION vibration;
	int vibration_left_total;
	int vibration_right_total;
	clock_t vibration_lastTimestamp;
	std::list<SRumble> vibration_list;
	float normalized_thumb_lx;
	float normalized_thumb_ly;
	float magnitude_left_thumb;
	float normalized_thumb_rx;
	float normalized_thumb_ry;
	float magnitude_right_thumb;
	float normalized_trigger_lt;
	float normalized_trigger_rt;
	clock_t timeAtLastConnectivityCheck;
};

class DXGamepadInput : public IInputDevice
{
public:
	DXGamepadInput();
	virtual ~DXGamepadInput() { CleanUp(); }

	virtual bool Init();
	virtual void GetInputState();

	void Rumble(int id, bool highFrequence, float strength, clock_t duration);

private:
	void SetRumbleStrength(int id, bool highFrequence, uint strength, bool substract);
	void UpdateRumbleStates();
	void NormalizeAnalogueInput(int id);
	void NormalizeThumbStick(int sThumbLX, int sThumbLY,
		float* normalizedMagnitude, float* normalizedX, float* normalizedY, uint thumb_deadzone);
	
	CONTROLLER_STATE m_Controllers[MAX_CONTROLLERS];

	virtual void CleanUp();
};

#endif

#endif // _DXGAMEPADINPUT_H_
