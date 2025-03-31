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
	if (attack_accuracity(gen) >= accuracity)
		return;
	float totalDamage = 0;
	for (size_t i = 0; i < target.size(); i++)
	{
		target[i]->stats.Attack.multiplier *= damage_multiplier;
		target[i]->stats.CriticalHit.multiplier *= critical_damage_multiplier;

		float attackDamage = max(1, (int)floor(damage * attacker.stats.Attack.GetProcessedValue() * (attacker.stats.CriticalHit.GetProcessedValue()*(critical_percentage(gen) < critical_damage_percentage ? critical_damage_multiplier:1.f)) / target[i]->stats.Defense.GetProcessedValue() * random(gen)));

		float attackDefense = defense;

		float attackHealth = target[i]->stats.health + extraHealth * extraHealth_multiplier;

		target[i]->stats.Poison.value = poisonDamage;
		target[i]->stats.Poison.turns = poisonTurns;

		target[i]->stats.Burn.value = burnDamage;
		target[i]->stats.Burn.turns = burnTurns;

		target[i]->stats.Regeneration.value = regenerationHealth;
		target[i]->stats.Regeneration.turns = regenerationTurns;

		totalDamage += attackDamage;

		target[i]->stats.turnBlocked = blockTurn;
		if (target[i]->stats.health > target[i]->stats.maxHealth)target[i]->stats.health = target[i]->stats.maxHealth;
		target[i]->stats.Defense.multiplier += attackDefense;
		if (target[i]->stats.Defense.multiplier > 3)target[i]->stats.Defense.multiplier = 3;
	}
	attacker.stats.health += healthStolen + totalDamage * healthReceived_percentage;
}
