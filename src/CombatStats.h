#pragma once
#include <vector>
struct CombatStats
{
	int life = 0;
	int maxLife = 0;
	float speed = 0;
	std::vector<int> attackListIndexes;
};