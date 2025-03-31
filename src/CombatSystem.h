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
		Enemy,
		Both,
		None
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
		int health=100;
		Stat attack;
		Stat defense;
		Stat speed;
		unordered_map<string,pair<Stat, int>> turnHealthModifier;
		void GetBaseStatsById(int id)
		{
			/// Get stats
		};
	};

	struct CharacterReference
	{
		int id;
		CharacterType team;
		CharacterCombatStats stats;

		CharacterReference() {};
		CharacterReference(int _id, CharacterType _team) {
			id = _id;
			team = _team;
			stats = CharacterCombatStats();
			stats.GetBaseStatsById(id);
		}
	};

	struct TurnAttack
	{
		Attack* attack;
		vector<CharacterReference*> targets;
	};

	CombatSystem();
	void AddPartyToCombat(const vector<int>& party, CharacterType party_type);
	void AddAttack(Attack* attack, CharacterReference& attacker, vector<CharacterReference*> targets);
	CombatState GetCombatState();
	void StartCombat();
	void UpdateCombat();
	void EndCombat();
	void ChangeState(CombatState newState);

	const unordered_map <CharacterType, vector<CharacterReference>>& GetCharactersInCombat();
	
	~CombatSystem();
private:
	void CheckDeadCharacters();
	unordered_map <CharacterType, vector<CharacterReference>> charactersInCombat;
	int turn = 0;
	CombatState state = CombatState::START;
	vector<pair<CharacterReference*, TurnAttack>> attackList;
};