#pragma once
#include "CombatSystem.h"
#include <vector>
using namespace std;


class CombatAI
{
private:

	pair<int,pair<int, vector<CombatSystem::CharacterReference*>>> bestOption;
	void CalculateBestOption(CombatSystem::CharacterReference* attacker, unordered_map <CombatSystem::CharacterType, vector<CombatSystem::CharacterReference>>& charactersInCombat);
	CombatSystem* combatSystem;
public:
	CombatAI(CombatSystem* system);
	~CombatAI();
	void CalculateAI(unordered_map <CombatSystem::CharacterType, vector<CombatSystem::CharacterReference>>& charactersInCombat);
};