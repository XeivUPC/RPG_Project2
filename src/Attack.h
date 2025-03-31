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
	TargetSelection selection = Random;
	int minTargetAmmount = 0;
	int maxTargetAmmount = 0;

	int id = -1;
	string name = "Name";
	string description = "Description";

	/// Stats
	int priority = 0;
	int accuracity = 100;

	/// Damage
	int damage = 0;
	int critRate = 10;

	/// Multipliers
	int damageIncrement = 0;
	int defenseIncrement = 0;
	int speedIncrement = 0;

	/// Effects
	int poisonDamage = 0;
	int poisonTurns = 0;

	int burnDamage = 0;
	int burnTurns = 0;

	int regenerationValue = 0;
	int regenerationTurns = 0;

	/// Life Steal
	int lifeSteal = 0;
	int lifeStealPercentage = 0;
	int lifeStealEffectiveness = 100;

	/// Extra
	bool blockTurn = false;

	virtual void DoAttack(CombatSystem::CharacterReference& attacker, std::vector<CombatSystem::CharacterReference*>& target);
};