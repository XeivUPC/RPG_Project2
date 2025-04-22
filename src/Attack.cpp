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
	std::uniform_int_distribution stat_or_status_porbability(0, 100);
	std::uniform_int_distribution attack_accuracy(1, 100);



	if (attack_accuracy(gen) > accuracy)
		return;

	CombatSystem::CharacterReference* attackerPtr = &attacker;

	int attaker_level = attackerPtr->stats.level;
	for (size_t i = 0; i < target.size(); i++)
	{

		int target_level = target[i]->stats.level;

		//// Check Dmg
		bool hasCritic = critical_percentage(gen) < (critRate);
		float criticMultiplier = hasCritic ? 1.5f : 1.f;

		float attackStat = attackerPtr->stats.GetStatProcessedValue(attackerPtr->stats.currentStats.attack, attackerPtr->stats.statsStages.attack);
		float defenseStat = target[i]->stats.GetStatProcessedValue(target[i]->stats.currentStats.defense, target[i]->stats.statsStages.defense);

		float damageToDo = ((((2.0f * attackerPtr->stats.level / 5.0f) + 2.0f) * power * attackStat / defenseStat) / 50.0f + 2.0f);
		damageToDo *= random(gen);

		if (power == 0)
			damageToDo = 0;

		float damageDone = target[i]->stats.currentHp;
		target[i]->stats.currentHp = max(0.f, target[i]->stats.currentHp - damageToDo);
		damageDone = damageDone - target[i]->stats.currentHp;

		switch (lifeDewMode)
		{
			case 0:
				attackerPtr->stats.currentHp += lifeDewAmount * (lifeDewEffectiveness/100.f);
				break;
			case 1:
				attackerPtr->stats.currentHp += damageDone * (lifeDewEffectiveness / 100.f);
				break;
			case 2:
				attackerPtr->stats.currentHp += target[i]->stats.currentStats.hp * (lifeDewAmount/100.f) * (lifeDewEffectiveness / 100.f);
				break;
			default:
				break;
		}

		if (blockTurnTarget == CombatSystem::Enemy) {
			target[i]->stats.isBlocked = blocksTurn;
		}
		else
			attackerPtr->stats.isBlocked = blocksTurn;

		/// 
		for (const auto& statModification : statsModification)
		{
			if (stat_or_status_porbability(gen) > statModification.second.probability)
				continue;

			CombatSystem::CharacterReference* character = &attacker;
			if (statModification.second.objective == CombatSystem::Enemy) {
				character = target[i];	
			}

			const string& type = statModification.second.type;
			if (type == "Attack") {
				character->stats.statsStages.attack += statModification.second.value;
				character->stats.statsStages.attack = (float)character->stats.statsStages.CheckCap(character->stats.statsStages.attack);
			}
			else if (type == "Defense") {
				character->stats.statsStages.defense += statModification.second.value;
				character->stats.statsStages.defense = (float)character->stats.statsStages.CheckCap(character->stats.statsStages.defense);
			}
			else if (type == "Speed") {
				character->stats.statsStages.speed += statModification.second.value;
				character->stats.statsStages.speed = (float)character->stats.statsStages.CheckCap(character->stats.statsStages.speed);
			}
		}

		for (const auto& statusModification : statusEffects)
		{
			if (stat_or_status_porbability(gen) > statusModification.second.probability)
				continue;

			CombatSystem::CharacterReference* character = &attacker;
			if (statusModification.second.objective == CombatSystem::Enemy) {
				character = target[i];	
			}

			const string& type = statusModification.second.type;
			bool hasEffect = false;
			for (size_t i = 0; i < character->stats.statusEffects.size(); i++)
			{
				if (character->stats.statusEffects[i].name == statusModification.second.type) {
					character->stats.statusEffects[i].turns = statusModification.second.turns;
					if (statusModification.second.mode == 0)
						character->stats.statusEffects[i].value = statusModification.second.value;
					else
						character->stats.statusEffects[i].value = character->stats.currentStats.hp * statusModification.second.value / 100.f;
					hasEffect = true;
					break;
				}
			}
			CombatSystem::StatusEffect* effect = nullptr;
			if (!hasEffect) {
				if (statusModification.second.mode == 0)
					effect = &character->stats.statusEffects.emplace_back(CombatSystem::StatusEffect{ type ,statusModification.second.value ,statusModification.second.turns });
				else
					effect = &character->stats.statusEffects.emplace_back(CombatSystem::StatusEffect{ type , character->stats.currentStats.hp * statusModification.second.value / 100.f,statusModification.second.turns });
			}
			if (effect == nullptr)
				continue;
			if (statusModification.second.activatesInstantly) {
				effect->turns--;
				character->stats.currentHp += effect->value;

				if (character->stats.currentHp < 0)
					character->stats.currentHp = 0.f;
				if (character->stats.currentHp > character->stats.currentStats.hp)
					character->stats.currentHp = (float)character->stats.currentStats.hp;
			}
		}
	}
}
