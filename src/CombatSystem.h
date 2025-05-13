#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "SystemEvent.h"
#include "CharacterDatabase.h"

using namespace std;
class Attack;
class CombatAI;

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
		PREPARE_ATTACKS,
		ATTACKS,
		EFFECTS,
		END_CHECK,
		VICTORY,
		DEFEAT
	};

	struct StatStages {
		float attack=0;
		float defense=0;
		float speed=0;

		int maxminLevel = 6;

		int CheckCap(float value) {
			if (value > maxminLevel)
				return maxminLevel;
			if (value < -maxminLevel)
				return -maxminLevel;
			return (int)value;
		}
	};

	struct BaseStats {
		int hp=0;
		int attack = 0;
		int defense = 0;
		int speed = 0;
	};

	struct StatusEffect {
		string name;
		float value = 0;
		int turns = 0;
	};

	struct CharacterStats {
		float currentHp = 0;
		int level = 0;

		bool isBlocked = false;

		BaseStats baseStats;
		BaseStats currentStats;
		StatStages statsStages;
		vector<StatusEffect> statusEffects;

		bool HasStatusEffect(string type) {
			for (size_t i = 0; i < statusEffects.size(); i++)
			{
				if (statusEffects[i].name == type)
					return true;
			}
			return false;
		}

		void Reset() {
			currentStats.hp = (int)GetHpStatValue();
			currentHp = (float)currentStats.hp;

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

		float GetStatProcessedValue(int value, float stage) {
			float stageExtra = (abs(stage) + 2) / 2.f;
			if (stage < 0)
				stageExtra = 1.f / stageExtra;
			return value*stageExtra;
		}
	};

	struct CharacterReference
	{
		int id=-1;
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
	int CurrentAttackIndex();
	bool CurrentAttackEnded();
	void NextAttack();
	CharacterReference* GetCurrentAttackAttacker();
	TurnAttack* GetCurrentTurnAttack();
	vector<CharacterReference*> GetCurrentAttackTargetList();
	int CurrentAttackTargetAmount();
	const unordered_map <CharacterType, vector<CharacterReference>>& GetCharactersInCombat();
	vector<CharacterReference*> GetPosibleTargets(CharacterReference* character, Attack* attack);
	
	~CombatSystem();
private:
	void CheckDeadCharacters();
	unordered_map <CharacterType, vector<CharacterReference>> charactersInCombat;
	int turn = 0;
	CombatState state = CombatState::START;
	vector<pair<CharacterReference*, TurnAttack>> attackList;
	int currentAttackIndex = 0;
	bool currentAttackEnded = false;
	CombatAI* ai;
};