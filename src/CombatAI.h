#pragma once
#include <vector>
using namespace std;

class CombatSystem::CharacterReference;

class CombatAI
{
private:
	pair<int,pair<int, vector<CombatSystem::CharacterReference*>>> bestOption;
	vector<CombatSystem::CharacterReference*> characterStats;
	void CalculateBestOption(CombatSystem::CharacterReference* attacker);
public:
	CombatAI();
	~CombatAI();
	pair<int, vector<CombatSystem::CharacterReference*>> GetBestOption(CombatSystem::CharacterReference* attacker, vector<CombatSystem::CharacterReference*> charactersInCombat);
};