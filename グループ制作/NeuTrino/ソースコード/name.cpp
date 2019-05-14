#include "name.h"
#include "main.h"

#include <fstream>

static std::string g_sPlayerName;

void SetName(std::string player_name)
{
	g_sPlayerName = "";
	g_sPlayerName = player_name;
	if (!GetTestModeMemberStatus(TMML_GUEST))
	{
		std::ofstream output_file(PLAYERNAME_FILE);
		output_file << player_name.c_str();
	}
}

std::string GetName()
{
	if (!GetTestModeMemberStatus(TMML_GUEST))
	{
		std::ifstream load_file(PLAYERNAME_FILE);
		std::string name;
		std::getline(load_file, name);
		return name;
	}
	else
	{
		return g_sPlayerName;
	}
}