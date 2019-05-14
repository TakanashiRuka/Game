#pragma once

#include <Windows.h>

class KeyBoard
{
	friend class Game;

private:
	static BYTE old_key_[256];
	static BYTE key_[256];

public:
	static bool Trigger(BYTE key);
	static bool Press(BYTE key);
	static bool Release(BYTE key);

private:
	static void Update();
};