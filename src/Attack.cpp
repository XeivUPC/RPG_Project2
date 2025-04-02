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
	std::uniform_int_distribution attack_accuracy(1, 100);



	if (attack_accuracy(gen) > accuracy)
		return;

	int attaker_level = attacker.stats.level;
	for (size_t i = 0; i < target.size(); i++)
	{

		int target_level = target[i]->stats.level;

		//// Check Dmg
		bool hasCritic = critical_percentage(gen) < (critRate);
		float criticMultiplier = hasCritic ? 1.5f : 1.f;

		int attackStat = attacker.stats.GetStatProcessedValue(attacker.stats.baseStats.attack, attacker.stats.statsStages.attack);
		int defenseStat = target[i]->stats.GetStatProcessedValue(target[i]->stats.baseStats.defense, target[i]->stats.statsStages.defense);

		float damageToDo = ((2 * attacker.stats.level / 5 + 2) * power * attackStat / defenseStat / 50 + 2) * random(gen);

		int damageDone = target[i]->stats.currentHp;
		target[i]->stats.currentHp = max(0, target[i]->stats.currentHp - (int)damageToDo);
		damageDone = damageDone - target[i]->stats.currentHp;

		switch (lifeDewMode)
		{
			case 0:
				attacker.stats.currentHp += lifeDewAmount * (lifeDewEffectiveness/100);
				break;
			case 1:
				attacker.stats.currentHp += damageDone * (lifeDewEffectiveness / 100);
				break;
			default:
				break;
		}

		if (blockTurnTarget == CombatSystem::Enemy) {
			target[i]->stats.isBlocked = blocksTurn;
		}
		else
			attacker.stats.isBlocked = blocksTurn;

		/// 
		for (const auto& statModification : statsModification)
		{
			CombatSystem::CharacterReference& character = attacker;
			if (statModification.second.objective == CombatSystem::Enemy) {
				character = *target[i];

				const string& type = statModification.second.type;
				if (type=="Attack") {
					character.stats.statsStages.attack += statModification.second.value;
				}else if (type == "Defense") {
					character.stats.statsStages.defense += statModification.second.value;
				}
				else if (type == "Speed") {
					character.stats.statsStages.speed += statModification.second.value;
				}
			}
		}

		for (const auto& statusModification : statusEffects)
		{
			CombatSystem::CharacterReference& character = attacker;
			if (statusModification.second.objective == CombatSystem::Enemy) {
				character = *target[i];

				const string& type = statusModification.second.type;
				bool hasEffect = false;
				for (size_t i = 0; i < character.stats.statusEffects.size(); i++)
				{
					if (character.stats.statusEffects[i].name == statusModification.second.type) {
						character.stats.statusEffects[i].turns = statusModification.second.turns;
						character.stats.statusEffects[i].value = statusModification.second.value;
						hasEffect = true;
						break;
					}
				}
				if (!hasEffect) {
					character.stats.statusEffects.emplace_back(CombatSystem::StatusEffect{type ,statusModification.second.turns ,statusModification.second.value});
				}
			}
		}



	}
}
