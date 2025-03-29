#include "CombatSystem.h"
#include "Attack.h"
#include <algorithm>

CombatSystem::CombatSystem()
{
}

void CombatSystem::AddPartyToCombat(vector<int> party, CharacterType party_type)
{
	charactersInCombat[party_type].clear();
	for (size_t i = 0; i < party.size(); i++)
	{
		charactersInCombat[party_type].emplace_back(
			CharacterCombatStats
			{
				party[i]
			});
		charactersInCombat[party_type][i].GetBaseStatsById();
	}
}

void CombatSystem::AddAttack(Attack* attack, CharacterReference& attacker, vector<CharacterReference&> targets)
{
	CharacterCombatStats& attackerData = charactersInCombat[attacker.team][attacker.position];
	vector<CharacterCombatStats&> targetData;
	for (size_t i = 0; i < targets.size(); i++)
	{
		CharacterReference& reference = targets[i];
		CharacterCombatStats& tempData = charactersInCombat[reference.team][reference.position];
		targetData.emplace_back(tempData);
	}
	attackList.emplace_back(pair<CharacterCombatStats&, TurnAttack>(attackerData, TurnAttack{ attack,targetData }));
}

CombatSystem::CombatState CombatSystem::GetCombatState()
{
	return state;
}

void CombatSystem::StartCombat()
{
	state = CombatState::START;
	
}

void CombatSystem::UpdateCombat()
{
	switch (state)
	{
	case CombatSystem::START:
		attackList.clear();
		ChangeState(CombatState::PLAYER_TURN);
		break;
	case CombatSystem::PLAYER_TURN:

		break;
	case CombatSystem::ENEMY_TURN:

		ChangeState(CombatState::ATTACKS);
		break;
	case CombatSystem::ATTACKS:
		sort(attackList.begin(), attackList.end(),
		[](pair< CharacterCombatStats&, Attack*>& a, pair< CharacterCombatStats&, Attack*>& b) {
			Stat as = a.first.speed;
			Stat bs = b.first.speed;
			float aSpeed = as.defaultValue * as.multiplier;
			float bSpeed = bs.defaultValue * bs.multiplier;

			auto getGroup = [](int priority) {
				if (priority > 0) return 1;    
				if (priority == 0) return 2;   
				return 3;                      
			};

			int group_a = getGroup(a.second->priority);
			int group_b = getGroup(b.second->priority);

			if (group_a != group_b) {
				return group_a < group_b; 
			}

			if (a.second->priority != b.second->priority) {
				return a.second->priority > b.second->priority;
			}
			return aSpeed > bSpeed;
		});
		for (auto& attack : attackList)
		{
			if (attack.first.health <= 0)
				continue;
			attack.second.attack->DoAttack(attack.first,attack.second.targets);
			
		}
		CheckDeadCharacters();
		ChangeState(CombatState::EFFECTS);
		break;
	case CombatSystem::EFFECTS:
		for (auto& team : charactersInCombat)
		{
			for (auto& character : team.second)
			{
				if (character.health <= 0)
					continue;
				for (auto& effect : character.turnHealthModifier)
				{
					if (effect.second.second > 0)
					{
						effect.second.second--;
						character.health += effect.second.first.defaultValue;
					}
				}
			}
		}
		CheckDeadCharacters();
		ChangeState(CombatState::END_CHECK);
		break;
	case CombatSystem::END_CHECK:
		for (auto& team : charactersInCombat)
		{
			bool allDead = team.second.size() == 0;
			if (allDead)
			{
				if (team.first == Ally)
					ChangeState(DEFEAT);
				else
					ChangeState(VICTORY);
			}
		}
		break;
	case CombatSystem::VICTORY:
		break;
	case CombatSystem::DEFEAT:
		break;
	default:
		break;
	}
}

void CombatSystem::EndCombat()
{
}

CombatSystem::~CombatSystem()
{
}

void CombatSystem::CheckDeadCharacters()
{
	for (auto& team : charactersInCombat)
	{
		for (auto& character : team.second)
		{
			if (character.health <= 0)
			{
				//Puede dar error APT APT APT APT
				team.second.erase(std::remove(team.second.begin(), team.second.end(), character), team.second.end());
			}
		}
	}
}

void CombatSystem::ChangeState(CombatState newState)
{
	if (state != newState)
	{
		state = newState;
		onCombatStateChanged.Trigger();
	}
}
