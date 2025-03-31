#include "CombatSystem.h"

class Attack
{
public:
	enum TargetSelection
	{
		Input,
		Random,
	};

	CombatSystem::CharacterType targetType = CombatSystem::Enemy;
	TargetSelection selection;
	int minTargetAmmount = 0;
	int maxTargetAmmount = 0;

	string name = "Name";
	string description = "Description";

	int priority = 0;
	int accuracity = 100;

	float damage = 0;
	float damage_multiplier = 1;
	float critical_damage_multiplier = 1;
	int critical_damage_percentage = 0;

	float defense = 0;
	float damage_percentage = 1;

	float extraHealth = 0;
	float extraHealth_multiplier = 1;

	float poisonDamage = 0;
	int poisonTurns = 0;

	float burnDamage = 0;
	int burnTurns = 0;

	float regenerationHealth = 0;
	int regenerationTurns = 0;

	float healthStolen = 0;
	float healthReceived_percentage = 100;

	bool blockTurn = false;

	virtual void DoAttack(CombatSystem::CharacterReference& attacker, std::vector<CombatSystem::CharacterReference*>& target);
};