#ifndef _NAME_H_
#define _NAME_H_

#include <string>

#define PLAYERNAME_FILE	("Data/PlayerName.txt")

void SetName(std::string player_name);
std::string GetName();

#endif // !_NAME_H_