#include "..\input\x_input.h"

#pragma comment(lib, "xinput.lib")

WORD XInput::old_buttons_;
XINPUT_STATE XInput::state_;

bool XInput::use_;

constexpr short threshold = 65535 / 4;

bool XInput::Trigger(int button)
{
	return (!(XInput::old_buttons_ & button) && (XInput::state_.Gamepad.wButtons & button)) ? true : false;
}

bool XInput::Press(int button)
{
	return (XInput::state_.Gamepad.wButtons & button) ? true : false;
}

bool XInput::Release(int button)
{
	return ((XInput::old_buttons_ & button) && !(XInput::state_.Gamepad.wButtons & button)) ? true : false;
}

bool XInput::ThumbLXLeft()
{
	return (XInput::state_.Gamepad.sThumbLX < -threshold);
}

bool XInput::ThumbLXRight()
{
	return (XInput::state_.Gamepad.sThumbLX > threshold);
}

bool XInput::ThumbLYUp()
{
	return (XInput::state_.Gamepad.sThumbLY > threshold);
}

bool XInput::ThumbLYDown()
{
	return (XInput::state_.Gamepad.sThumbLY < -threshold);
}

bool XInput::ThumbRXLeft()
{
	return (XInput::state_.Gamepad.sThumbRX < -threshold);
}

bool XInput::ThumbRXRight()
{
	return (XInput::state_.Gamepad.sThumbRX > threshold);
}

bool XInput::ThumbRYUp()
{
	return (XInput::state_.Gamepad.sThumbRY > threshold);
}

bool XInput::ThumbRYDown()
{
	return (XInput::state_.Gamepad.sThumbRY < -threshold);
}

void XInput::Update()
{
	old_buttons_ = XInput::state_.Gamepad.wButtons;
	DWORD result = XInputGetState(0, &XInput::state_);
	(result == ERROR_SUCCESS)
		? XInput::use_ = true
		: XInput::use_ = false;
}

bool XInput::Use()
{
	return XInput::use_;
}