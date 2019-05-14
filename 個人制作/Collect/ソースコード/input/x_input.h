#pragma once

#include <Windows.h>
#include <Xinput.h>

class XInput
{
	friend class Game;

private:
	static WORD old_buttons_;
	static XINPUT_STATE state_;

	static bool use_;

public:
	static bool Trigger(int button);
	static bool Press(int button);
	static bool Release(int button);
	static bool ThumbLXLeft();
	static bool ThumbLXRight();
	static bool ThumbLYUp();
	static bool ThumbLYDown();
	static bool ThumbRXLeft();
	static bool ThumbRXRight();
	static bool ThumbRYUp();
	static bool ThumbRYDown();

	static bool Use();

private:
	static void Update();
};