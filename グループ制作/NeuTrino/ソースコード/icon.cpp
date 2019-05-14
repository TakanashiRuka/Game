#include "icon.h"
#include "main.h"

#include <fstream>
#include <string>

static int g_nIconNum;

void SetIcon()
{
	std::ifstream load_file("Data/Icon.txt");
	std::string icon;
	std::getline(load_file, icon);
	g_nIconNum = atoi(icon.c_str());
}

int GetIcon()
{
	if (GetGameMode() == MODE_DEMO)
	{
		return 0;
	}
	else
	{
		return g_nIconNum;
	}
}