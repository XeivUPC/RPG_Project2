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
	int accuracy = 100;

	/// Damage
	int damage = 0;
	int critRate = 10;

	/// Multipliers
	int damageIncrementToTarget = 0;
	int defenseIncrementToTarget = 0;
	int speedIncrementToTarget = 0;
	
	int damageIncrementToAttacker = 0;
	int defenseIncrementToAttacker = 0;
	int speedIncrementToAttacker = 0;

	/// Effects
	int poisonDamageToTarget = 0;
	int poisonTurnsToTarget = 0;
	int poisonDamageToAttacker = 0;
	int poisonTurnsToAttacker = 0;

	int burnDamageToTarget = 0;
	int burnTurnsToTarget = 0;
	int burnDamageToAttacker = 0;
	int burnTurnsToAttacker = 0;

	int regenerationValueToTarget = 0;
	int regenerationTurnsToTarget = 0;
	int regenerationValueToAttacker = 0;
	int regenerationTurnsToAttacker = 0;

	/// Life Steal
	int lifeSteal = 0;
	int lifeStealPercentage = 0;
	int lifeStealEffectiveness = 100;

	/// Extra
	bool blockTurnToTarget = false;
	bool blockTurnToAttacker = false;

	virtual void DoAttack(CombatSystem::CharacterReference& attacker, std::vector<CombatSystem::CharacterReference*>& target);
};