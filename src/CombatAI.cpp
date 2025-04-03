#include "CombatAI.h"
#include "CombatSystem.h"
#include "CharacterDatabase.h"
#include "AttackList.h"

void CombatAI::CalculateBestOption(CombatSystem::CharacterReference* attacker)
{
	CharacterDatabase::CharacterData& attackerData = CharacterDatabase::Instance().GetCharacterData(attacker->id);
	for (size_t i = 0; i < attackerData.attacks.size(); i++)
	{
		int efficiency = 0;
		vector<pair<int,int>> TargetEfficiencyList;
		Attack* attack = AttackList::Instance().GetAttack(attackerData.attacks[i]);
		for (size_t j = 0; j < characterStats.size(); j++)
		{
			int singleDamageEfficiency = 0;
			int singleDefenseEfficiency = 0;
			int singleHealthEfficiency = 0;
			pair<CombatSystem::CharacterType, CombatSystem::CharacterType> relation = pair<CombatSystem::CharacterType, CombatSystem::CharacterType>(attack->targetType, characterStats[i]->team);
			if (relation.first == CombatSystem::Enemy && relation.second == CombatSystem::Enemy)
				continue;

			if (relation.first == CombatSystem::Ally && relation.second == CombatSystem::Ally)
				continue;

			if (relation.first == CombatSystem::Self && characterStats[j] != attacker)
				continue;

			/// ------------------------------------ Rules Start

			//Damage Efficiency
			singleDamageEfficiency += (characterStats[j]->stats.currentHp == attack->power ? 2 : 1);
			singleDamageEfficiency += (characterStats[j]->stats.currentHp == attack->statusEffects["Poison"].value*attack->statusEffects["Poison"].turns ? 2 : 1);
			singleDamageEfficiency += (characterStats[j]->stats.currentHp == attack->statusEffects["Burn"].value*attack->statusEffects["Burn"].turns ? 2 : 1);

			//Defense Efficiency
			if (attack->statsModification["Defense"].value < 0)
			{
				singleDefenseEfficiency += (characterStats[j]->stats.currentStats.defense - characterStats[j]->stats.baseStats.defense == attack->statsModification["Defense"].value ? 2:1);
				singleDefenseEfficiency += (characterStats[j]->stats.currentStats.attack > attacker->stats.currentStats.defense ? 1 : 0);
				singleDefenseEfficiency += (characterStats[j]->stats.currentHp * 2 == characterStats[j]->stats.currentStats.hp ? 2 : 1);
			}
			else
			{
				singleDefenseEfficiency += (characterStats[j]->stats.currentHp * 2 == characterStats[j]->stats.currentStats.hp ? 2 : 1);
				singleDefenseEfficiency += (/*characterStats[j]->stats.statsStages.maxDefense+*/characterStats[j]->stats.currentStats.defense == attack->statsModification["Defense"].value ? 2 : 1);
			}

			//Health Efficiency
			singleHealthEfficiency += (characterStats[j]->stats.currentHp == attack->statusEffects["Regeneration"].value * attack->statusEffects["Regeneration"].turns ? 2 : 1);
			
			/// ------------------------------------ Rules End

			int singleEfficiency = singleDamageEfficiency + singleDefenseEfficiency + singleHealthEfficiency;
			TargetEfficiencyList.emplace_back(pair<int,int>((int)j,singleEfficiency));
			efficiency += singleEfficiency;
		}
		while (TargetEfficiencyList.size() > attack->targetAmount)
		{
			int min = 0;
			for (size_t j = 1; j < TargetEfficiencyList.size(); j++)
			{
				if (TargetEfficiencyList[min].second > TargetEfficiencyList[j].second)
					min = (int)j;
			}
			efficiency -= TargetEfficiencyList[min].second;
			TargetEfficiencyList.erase(TargetEfficiencyList.begin() + min);
		}
		if (bestOption.first < efficiency)
		{
			bestOption.first = efficiency;
			bestOption.second.first = (int)i;
			for (size_t i = 0; i < TargetEfficiencyList.size(); i++)
			{
				bestOption.second.second.emplace_back(characterStats[i]);
			}
		}
	}
}

CombatAI::CombatAI()
{
}

CombatAI::~CombatAI()
{
}

pair<int, vector<CombatSystem::CharacterReference*>> CombatAI::GetBestOption(CombatSystem::CharacterReference* attacker, vector<CombatSystem::CharacterReference*> charactersInCombat)
{
	characterStats = charactersInCombat;
	CalculateBestOption(attacker);
	return bestOption.second;
}
