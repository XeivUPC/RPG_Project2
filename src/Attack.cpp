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
	std::uniform_int_distribution attack_accuracity(0, 100);



	if (attack_accuracity(gen) > accuracity)
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
		attacker.stats.Attack.AddMultiplier(damageIncrement);
		attacker.stats.Defense.AddMultiplier(defenseIncrement);
		attacker.stats.Speed.AddMultiplier(speedIncrement);

		//// SetEffects
		attacker.stats.Poison.currentValue = poisonDamage;
		attacker.stats.Poison.turns = poisonTurns;

		attacker.stats.Burn.currentValue = burnDamage;
		attacker.stats.Burn.turns = burnTurns;

		attacker.stats.Regeneration.currentValue = regenerationValue;
		attacker.stats.Regeneration.turns = regenerationTurns;

		//// Other
		if(!target[i]->stats.turnBlocked)
			target[i]->stats.turnBlocked = blockTurn;

	}
	if(attacker.stats.Health.currentValue> attacker.stats.Health.defaultValue)
		attacker.stats.Health.currentValue = attacker.stats.Health.defaultValue;

}
