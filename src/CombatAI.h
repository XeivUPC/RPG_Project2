#pragma once
#include "CombatSystem.h"
#include <vector>
using namespace std;


class CombatAI
{
private:
	pair<int,pair<int, vector<CombatSystem::CharacterReference*>>> bestOption;
	vector<CombatSystem::CharacterReference*> characterStats;
	void CalculateBestOption(CombatSystem::CharacterReference* attacker);

	CombatSystem* combatSystem;
public:
	CombatAI(CombatSystem* system);
	~CombatAI();
	void CalculateAI(vector<CombatSystem::CharacterReference*> charactersInCombat);
};