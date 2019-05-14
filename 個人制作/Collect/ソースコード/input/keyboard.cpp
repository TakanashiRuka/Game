#include "..\input\keyboard.h"

#include "..\input\x_input.h"

BYTE KeyBoard::old_key_[256] = {};
BYTE KeyBoard::key_[256] = {};

bool KeyBoard::Trigger(BYTE key)
{
	return (!(KeyBoard::old_key_[key] & 0x80) && (KeyBoard::key_[key] & 0x80)) ? true : false;
}

bool KeyBoard::Press(BYTE key)
{
	return (KeyBoard::key_[key] & 0x80) ? true : false;
}

bool KeyBoard::Release(BYTE key)
{
	return ((KeyBoard::old_key_[key] & 0x80) && !(KeyBoard::key_[key] & 0x80)) ? true : false;
}

void KeyBoard::Update()
{
	for (int key = 0; key < 256; ++key)
	{
		KeyBoard::old_key_[key] = KeyBoard::key_[key];
	}
	GetKeyboardState(KeyBoard::key_);

	if (XInput::Use())
		for (int key = 0; key < 256; ++key)
			KeyBoard::key_[key] = 0;
}