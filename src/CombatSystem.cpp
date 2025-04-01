#include "CombatSystem.h"
#include "Attack.h"
#include <algorithm>

CombatSystem::CombatSystem()
{
}

void CombatSystem::AddPartyToCombat(const vector<int>& party, CharacterType party_type)
{
	charactersInCombat[party_type].clear();
	for (size_t i = 0; i < party.size(); i++)
	{
		charactersInCombat[party_type].emplace_back(CharacterReference(party[i], party_type));
	}
}

void CombatSystem::AddAttack(Attack* attack, CharacterReference& attacker, vector<CharacterReference*> targets)
{
	attackList.emplace_back(pair<CharacterReference*, TurnAttack>(&attacker, TurnAttack{ attack,targets }));
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
		[](pair< CharacterReference*, TurnAttack>& a, pair< CharacterReference*, TurnAttack>& b) {
			Stat& as = a.first->stats.Speed;
			Stat& bs = b.first->stats.Speed;
			float aSpeed = as.GetProcessedValue(a.first->stats.level);
			float bSpeed = bs.GetProcessedValue(b.first->stats.level);

			auto getGroup = [](int priority) {
				if (priority > 0) return 1;    
				if (priority == 0) return 2;   
				return 3;                      
			};

			int group_a = getGroup(a.second.attack->priority);
			int group_b = getGroup(b.second.attack->priority);

			if (group_a != group_b) {
				return group_a < group_b; 
			}

			if (a.second.attack->priority != b.second.attack->priority) {
				return a.second.attack->priority > b.second.attack->priority;
			}
			return aSpeed > bSpeed;
		});
		for (auto& attack : attackList)
		{
			if (attack.first->stats.Health.currentValue <= 0)
				continue;
			attack.second.attack->DoAttack(*attack.first,attack.second.targets);
			
		}
		CheckDeadCharacters();
		ChangeState(CombatState::EFFECTS);
		break;
	case CombatSystem::EFFECTS:
		for (auto& team : charactersInCombat)
		{
			for (auto& character : team.second)
			{
				if (character.stats.Health.currentValue <= 0)
					continue;
				if (character.stats.Poison.turns > 0)
				{
					Stat& stat = character.stats.Poison;
					stat.turns--;
					character.stats.Health.currentValue -= stat.currentValue;
				}
				if (character.stats.Burn.turns > 0)
				{
					Stat& stat = character.stats.Burn;
					stat.turns--;
					character.stats.Health.currentValue -= stat.currentValue;
				}
				if (character.stats.Regeneration.turns > 0)
				{
					Stat& stat = character.stats.Health;
					stat.turns--;
					character.stats.Health.currentValue += stat.currentValue;
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
		if(state == END_CHECK)
			ChangeState(START);
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

	/// Do not remove, just take in care they are dead
	/*for (auto& team : charactersInCombat) {
		auto& characters = team.second;
		characters.erase(
			std::remove_if(
				characters.begin(),
				characters.end(),
				[](const CharacterReference& c) { return c.stats.Health.currentValue <= 0; }
			),
			characters.end()
		);
	}*/
}

void CombatSystem::ChangeState(CombatState newState)
{
	if (state != newState)
	{
		state = newState;
		onCombatStateChanged.Trigger();
	}
}

const unordered_map<CombatSystem::CharacterType, vector<CombatSystem::CharacterReference>>& CombatSystem::GetCharactersInCombat()
{
	return charactersInCombat;
}
