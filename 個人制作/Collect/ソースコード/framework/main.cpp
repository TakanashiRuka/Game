#include <Windows.h>
#include <crtdbg.h>

#include "..\framework\game.h"
#include "..\scene\title.h"

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto game = Game::GetInstance();
	game->Begin<Title>("Collect", 60U);
	game->Loop();
	game->End();
}