#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "SystemEvent.h"

using namespace std;
class Attack;
class CombatSystem
{
public:
	SystemEvent<> onCombatStateChanged;
	enum CharacterType
	{
		Ally,
		Enemy
	};
	enum CombatState
	{
		START,
		PLAYER_TURN,
		ENEMY_TURN,
		ATTACKS,
		EFFECTS,
		END_CHECK,
		VICTORY,
		DEFEAT
	};
	struct Stat
	{
		int defaultValue;
		float multiplier;
	};
	struct CharacterCombatStats
	{
		int id;
		int health;
		Stat attack;
		Stat defense;
		Stat speed;
		unordered_map<string,pair<Stat, int>> turnHealthModifier;
		void GetBaseStatsById()
		{

		};
	};

	struct CharacterReference
	{
		CharacterType team;
		int position;
	};

	struct TurnAttack
	{
		Attack* attack;
		vector<CharacterCombatStats&> targets;
	};

	CombatSystem();
	void AddPartyToCombat(vector<int> party, CharacterType party_type);
	void AddAttack(Attack* attack, CharacterReference& attacker, vector<CharacterReference&> targets);
	CombatState GetCombatState();
	void StartCombat();
	void UpdateCombat();
	void EndCombat();
	void ChangeState(CombatState newState);
	
	~CombatSystem();
private:
	void CheckDeadCharacters();
	unordered_map <CharacterType, vector<CharacterCombatStats>> charactersInCombat;
	int turn = 0;
	CombatState state = CombatState::START;
	vector<pair<CharacterCombatStats&, TurnAttack>> attackList;
};