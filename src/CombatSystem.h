#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "SystemEvent.h"
#include "CharacterDatabase.h"

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
		float value = 0;
		float multiplier = 1;
		int turns = 1;

		Stat(float _value, float _multiplier, int _turns)
		{
			value = _value;
			multiplier = _multiplier;
			turns = _turns;
		}
	};

	struct CharacterCombatStats
	{
		bool turnBlocked = false;
		float health = 100;

		Stat CriticalHit;
		Stat Attack;
		Stat Defense;
		Stat Speed;
		Stat Poison;
		Stat Burn;
		Stat Regeneration;
		
		void GetBaseStatsById(int id)
		{
			CharacterDatabase::CharacterData& reference = CharacterDatabase::Instance().GetCharacterData(id);
			health = reference.health;
			Attack.value = reference.attack;
			Defense.value = reference.defense;
			Speed.value = reference.speed;
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