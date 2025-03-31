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
		float defaultValue = 0;
		float currentValue = 0;

		float multiplier = 1;

		int turns = 1;
		
		Stat() = default;

		Stat(float _value, float _multiplier, int _turns)
		{
			defaultValue = _value;
			currentValue = _value;

			multiplier = _multiplier;
			turns = _turns;
		}
		float GetProcessedValue()
		{
			float multiplierRealValue = multiplier + 2;
			if (multiplier < 0) {
				return defaultValue * (2/ multiplierRealValue);
			}
			return defaultValue * (multiplierRealValue/2);
		}

		void AddMultiplier(float value)
		{
			multiplier += value;
			if (multiplier > 4)
				multiplier = 4;

			if (multiplier < -4)
				multiplier = -4;
		}
	};

	struct CharacterCombatStats
	{
		bool turnBlocked = false;

		Stat Health;

		Stat Attack;
		Stat Defense;
		Stat Speed;

		Stat Poison;
		Stat Burn;
		Stat Regeneration;


		CharacterCombatStats() = default;
		void GetBaseStatsById(int id)
		{
			CharacterDatabase::CharacterData& reference = CharacterDatabase::Instance().GetCharacterData(id);
			Health = Stat((float)reference.health, 1.f, 0);
			Attack = Stat((float)reference.attack, 1.f, 0);
			Defense = Stat((float)reference.defense, 1.f, 0);
			Speed = Stat((float)reference.speed, 1.f, 0);

			Poison = Stat(0, 1.f, 0);
			Burn = Stat(0, 1.f, 0);
			Regeneration = Stat(0, 1.f, 0);
		};


		
	};

	struct CharacterReference
	{
		int id;
		CharacterType team;
		CharacterCombatStats stats;

		CharacterReference() = default;
		CharacterReference(int _id, CharacterType _team) {
			id = _id;
			team = _team;
			stats.GetBaseStatsById(id);
		};
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