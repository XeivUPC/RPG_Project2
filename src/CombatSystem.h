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
		Enemy,
		Ally,
		Both,
		Self
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

	struct StatStages {
		int attack=0;
		int defense=0;
		int speed=0;

		int maxminLevel = 6;

		int CheckCap(int value) {
			if (value > maxminLevel)
				return maxminLevel;
			if (value < -maxminLevel)
				return -maxminLevel;
			return value;
		}
	};

	struct BaseStats {
		int hp;
		int attack;
		int defense;
		int speed;
	};

	struct StatusEffect {
		string name;
		int value;
		int turns;
	};

	struct CharacterStats {
		int currentHp;
		int level;

		bool isBlocked = false;

		BaseStats baseStats;
		BaseStats currentStats;
		StatStages statsStages;
		vector<StatusEffect> statusEffects;

		void Reset() {
			currentStats.hp = (int)GetHpStatValue();
			currentHp = currentStats.hp;

			currentStats.attack = (int)GetOtherStatValue(baseStats.attack);
			currentStats.defense = (int)GetOtherStatValue(baseStats.defense);
			currentStats.speed = (int)GetOtherStatValue(baseStats.speed);

			statsStages = { 0,0,0 };
			statusEffects.clear();
		}

		void GetBaseStatsById(int id) {
			CharacterDatabase::CharacterData& reference = CharacterDatabase::Instance().GetCharacterData(id);
			level = reference.level;

			baseStats.attack = reference.attack;
			baseStats.defense = reference.defense;
			baseStats.hp = reference.health;
			baseStats.speed = reference.speed;

			Reset();
		}

		float GetHpStatValue() {
			return floor(0.01f * (2 * baseStats.hp) * level) + level + 10;
		}

		float GetOtherStatValue(int defaultValue) {
			return floor(0.01f * (2 * defaultValue) * level) + 5;
		}

		float GetStatProcessedValue(int value, int stage) {
			float stageExtra = (stage + 2) / 2.f;
			if (stage < 0)
				stageExtra = 1 / stageExtra;
			return value*stageExtra;
		}
	};


	struct CharacterReference
	{
		int id;
		CharacterType team;
		CharacterStats stats;

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