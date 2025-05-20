#include "CombatAI.h"
#include "CombatSystem.h"
#include "CharacterDatabase.h"
#include "AttackList.h"
#include <random>

using namespace std;

void CombatAI::CalculateBestOption(CombatSystem::CharacterReference* attacker, unordered_map <CombatSystem::CharacterType, vector<CombatSystem::CharacterReference>>& charactersInCombat)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	bestOption.first = 0;
	bestOption.second.first = 0;
	bestOption.second.second.clear();

	CharacterDatabase::CharacterData& attackerData = CharacterDatabase::Instance().GetCharacterData(attacker->id);
	for (size_t i = 0; i < attackerData.attacks.size(); i++)
	{
		int efficiency = 0;


		//// Por cada pj, que tan efficiente es el ataque
		vector<pair<CombatSystem::CharacterReference*, int>> TargetEfficiencyList;


		Attack* attack = AttackList::Instance().GetAttack(attackerData.attacks[i]);

		int j = 0;
		for (auto& teamData : charactersInCombat)
		{
			for (auto& characterStats : teamData.second)
			{
				int singleDamageEfficiency = 0;
				int singleDefenseEfficiency = 0;
				int singleHealthEfficiency = 0;
				int singleAccuracyEfficiency = 0;
				pair<CombatSystem::CharacterType, CombatSystem::CharacterType> relation = pair<CombatSystem::CharacterType, CombatSystem::CharacterType>(attack->targetType, teamData.first);
				if (relation.first == CombatSystem::Enemy && relation.second == CombatSystem::Enemy)
					continue;

				if (relation.first == CombatSystem::Ally && relation.second == CombatSystem::Ally)
					continue;

				if (relation.first == CombatSystem::Self && &characterStats != attacker)
					continue;

				if (characterStats.stats.currentHp == 0)
					continue;

				/// ------------------------------------ Rules Start

				//Damage Efficiency
				if (attack->power != 0 || attack->statusEffects["Poison"].value != 0 || attack->statusEffects["Burn"].value != 0 || characterStats.stats.currentHp <= 0)
				{
					singleDamageEfficiency += attack->power;
					if(characterStats.stats.HasStatusEffect("Poison"))
						singleDamageEfficiency += (int)(attack->statusEffects["Poison"].value * attack->statusEffects["Poison"].turns);
					if (characterStats.stats.HasStatusEffect("Burn"))
						singleDamageEfficiency += (int)(attack->statusEffects["Burn"].value * attack->statusEffects["Burn"].turns);
				}
				//Defense Efficiency
				if (attack->statsModification["Defense"].value < 0)
				{
					singleDefenseEfficiency += (int)(attack->statsModification["Defense"].value);
					singleDefenseEfficiency += attacker->stats.currentStats.defense;
					singleDefenseEfficiency += characterStats.stats.currentStats.hp/2;
				}
				else if (attack->statsModification["Defense"].value > 0)
				{
					singleDefenseEfficiency += characterStats.stats.currentStats.hp;
					singleDefenseEfficiency += (int)(attack->statsModification["Defense"].value);
				}
				//Health Efficiency
				if (attack->statusEffects["Regeneration"].value != 0)
				{
					singleHealthEfficiency += (int)(attack->statusEffects["Regeneration"].value * attack->statusEffects["Regeneration"].turns);
				}
				singleAccuracyEfficiency += attack->accuracy;
				/// ------------------------------------ Rules End

				int singleEfficiency = singleDamageEfficiency + singleDefenseEfficiency + singleHealthEfficiency + singleAccuracyEfficiency;
				TargetEfficiencyList.emplace_back(pair<CombatSystem::CharacterReference*, int>(&characterStats, singleEfficiency));
				efficiency += singleEfficiency;

				j++;
			}
		}

		////// De estos pj que puedo atacar, cuales me rentan mas, seleccionamos los mas altos -> Nos da los mejores para este ataque

		while (TargetEfficiencyList.size() > attack->targetAmount)
		{
			int min = 0;
			for (size_t j = 1; j < TargetEfficiencyList.size(); j++)
			{
				if (TargetEfficiencyList[min].second == TargetEfficiencyList[j].second)
				{
					uniform_int_distribution preferenceToAttack(0, 1);
					bool equalEfficiencyRandom = (bool)preferenceToAttack(gen);
					if(equalEfficiencyRandom)
						min = (int)j;
				}
				else if (TargetEfficiencyList[min].second > TargetEfficiencyList[j].second)
					min = (int)j;
			}
			efficiency -= TargetEfficiencyList[min].second;
			TargetEfficiencyList.erase(TargetEfficiencyList.begin() + min);
		}
		
		if (bestOption.first < efficiency)
		{
			bestOption.first = efficiency;
			bestOption.second.first = attack->id;	
			bestOption.second.second.clear();
			for (size_t x = 0; x < TargetEfficiencyList.size(); x++)
			{
				bestOption.second.second.emplace_back(TargetEfficiencyList[x].first);
			}
		}
	}
}

CombatAI::CombatAI(CombatSystem* system)
{
	combatSystem = system;

}

CombatAI::~CombatAI()
{
}

void CombatAI::CalculateAI(unordered_map <CombatSystem::CharacterType, vector<CombatSystem::CharacterReference>>& charactersInCombat)
{
	for (auto& attackerData : charactersInCombat[CombatSystem::Enemy])
	{
		if (attackerData.stats.currentHp == 0)
			continue;
		CalculateBestOption(&attackerData, charactersInCombat);
		Attack* attack = AttackList::Instance().GetAttack(bestOption.second.first);
		combatSystem->AddAttack(attack, attackerData, bestOption.second.second);
	}
}
