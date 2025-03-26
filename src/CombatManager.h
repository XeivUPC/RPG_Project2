#pragma once
#include <vector>
class CombatManager
{
private:
	int turn = 0;
	std::vector<std::pair<int, bool>> combatActionList;
};