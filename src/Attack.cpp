#include "Attack.h"
#include <random>
#include <math.h>

using namespace std;
void Attack::DoAttack(CombatSystem::CharacterReference& attacker, std::vector<CombatSystem::CharacterReference*>& target)
{
	std::random_device rd;  // Obtain a random number from hardware
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> random(0.85f, 1.0);
	std::uniform_int_distribution critical_percentage(0, 100);
	std::uniform_int_distribution attack_accuracy(0, 100);



	if (attack_accuracy(gen) > accuracy)
		return;

	int attaker_level = attacker.stats.level;
	for (size_t i = 0; i < target.size(); i++)
	{

		int target_level = target[i]->stats.level;

		//// Check Dmg
		bool hasCritic = critical_percentage(gen) < (critRate);
		float criticMultiplier = hasCritic ? 1.5f : 1.f;

		float attackDamage = (float)max(1, (int)floor(( ((2 * attacker.stats.level / 5 + 2)) * damage * attacker.stats.Attack.GetProcessedValue(attaker_level) * criticMultiplier) / target[i]->stats.Defense.GetProcessedValue(target_level) * random(gen)));

		float removedHealth = target[i]->stats.Health.currentValue;
		target[i]->stats.Health.currentValue -= attackDamage;
		if (target[i]->stats.Health.currentValue < 0)
			target[i]->stats.Health.currentValue = 0;
		removedHealth = removedHealth - target[i]->stats.Health.currentValue;
		
		//// Check LifeSteal
		attacker.stats.Health.currentValue += removedHealth * (lifeStealPercentage / 100.f) * (lifeStealEffectiveness / 100.f);
		attacker.stats.Health.currentValue += lifeSteal * (lifeStealEffectiveness / 100.f);

		//// Increase Multipliers
		target[i]->stats.Attack.AddMultiplier(damageIncrementToTarget);
		target[i]->stats.Defense.AddMultiplier(defenseIncrementToTarget);
		target[i]->stats.Speed.AddMultiplier(speedIncrementToTarget);

		attacker.stats.Attack.AddMultiplier(damageIncrementToAttacker);
		attacker.stats.Defense.AddMultiplier(defenseIncrementToAttacker);
		attacker.stats.Speed.AddMultiplier(speedIncrementToAttacker);

		//// SetEffects
		target[i]->stats.Poison.currentValue = poisonDamageToTarget;
		target[i]->stats.Poison.turns = poisonTurnsToTarget;
		attacker.stats.Poison.currentValue = poisonDamageToAttacker;
		attacker.stats.Poison.turns = poisonTurnsToAttacker;

		target[i]->stats.Burn.currentValue = burnDamageToTarget;
		target[i]->stats.Burn.turns = burnTurnsToTarget;
		attacker.stats.Burn.currentValue = burnDamageToAttacker;
		attacker.stats.Burn.turns = burnTurnsToAttacker;

		target[i]->stats.Regeneration.currentValue = regenerationValueToTarget;
		target[i]->stats.Regeneration.turns = regenerationTurnsToTarget;
		attacker.stats.Regeneration.currentValue = regenerationValueToAttacker;
		attacker.stats.Regeneration.turns = regenerationTurnsToAttacker;

		//// Other
		if(!target[i]->stats.turnBlocked)
			target[i]->stats.turnBlocked = blockTurnToTarget;

		if (!attacker.stats.turnBlocked)
			attacker.stats.turnBlocked = blockTurnToAttacker;

	}
}
