#include "CombatSystem.h"

class Attack
{
private:
	

public:

	struct StageModifier {
		string type;

		float value = 0;
		int probability = 0;
		CombatSystem::CharacterType objective = CombatSystem::Ally;
	};

	struct StatusModifier {
		string type;

		float value = 0;
		int turns = 0;
		int mode = 0;
		int probability = 0;
		CombatSystem::CharacterType objective = CombatSystem::Ally;
	};

	enum AttackType
	{
		Aggressive,
		Healing,
		Buff,
		Debuff,
		Affliction
	};

	int id = -1;

	AttackType type = Aggressive;
	CombatSystem::CharacterType targetType = CombatSystem::Enemy;
	int targetAmount = 0;

	string name = "Name";
	string description = "Description";

	/// Properties
	int priority = 0;
	int accuracy = 100;

	/// Basic Stats
	int power = 0;
	int critRate = 0;

	/// Stages
	unordered_map<string, StageModifier> statsModification;

	/// Effects
	unordered_map<string, StatusModifier> statusEffects;

	/// Extra
	int lifeDewMode = -1;
	int lifeDewAmount = 0;
	int lifeDewEffectiveness=0;

	bool blocksTurn = false;
	CombatSystem::CharacterType blockTurnTarget = CombatSystem::Enemy;


	virtual void DoAttack(CombatSystem::CharacterReference& attacker, std::vector<CombatSystem::CharacterReference*>& target);
};