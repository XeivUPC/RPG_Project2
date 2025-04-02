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

	struct StatStages {
		int attack=0;
		int defense=0;
		int speed=0;
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
		StatStages statsStages;
		vector<StatusEffect> statusEffects;

		void GetBaseStatsById(int id) {
			CharacterDatabase::CharacterData& reference = CharacterDatabase::Instance().GetCharacterData(id);
			level = reference.level;

			baseStats.attack = reference.attack;
			baseStats.defense = reference.defense;
			baseStats.hp = reference.health;
			baseStats.speed = reference.speed;

			currentHp = baseStats.hp;
		}

		float GetStatProcessedValue(int value, int stage) {
			float stageExtra = (stage + 2) / 2.f;
			if (stage < 0)
				stageExtra = 1 / stageExtra;
			return (value + value*level/50.f)*stageExtra;
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