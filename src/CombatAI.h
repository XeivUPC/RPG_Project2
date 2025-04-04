#pragma once
#include "CombatSystem.h"
#include <vector>
using namespace std;


class CombatAI
{
private:
	pair<int,pair<int, vector<CombatSystem::CharacterReference*>>> bestOption;
	unordered_map <CombatSystem::CharacterType, vector<CombatSystem::CharacterReference>> characters;
	void CalculateBestOption(CombatSystem::CharacterReference* attacker);

	CombatSystem* combatSystem;
public:
	CombatAI(CombatSystem* system);
	~CombatAI();
	void CalculateAI(unordered_map <CombatSystem::CharacterType, vector<CombatSystem::CharacterReference>>& charactersInCombat);
};