#pragma once

#include <vector>

class CombatSystem
{
private:
	int turn = 0;
	std::vector<pair<int, bool>> npcActionCompletedList;
};