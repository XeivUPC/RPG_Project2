#include "CombatSystem.h"
#include "Attack.h"
#include "CombatAI.h"
#include "Engine.h"
#include "ModuleDebug.h"
#include <algorithm>

CombatSystem::CombatSystem()
{
	ai = new CombatAI(this);
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
	for (auto& team : charactersInCombat)
	{
		for (auto& character : team.second)
		{
			character.stats.Reset();
		}
	}
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
		if(!Engine::Instance().m_debug->godmode)
			ai->CalculateAI(charactersInCombat);
		ChangeState(CombatState::ATTACKS);
		break;
	case CombatSystem::ATTACKS:
		sort(attackList.begin(), attackList.end(),
		[](pair< CharacterReference*, TurnAttack>& a, pair< CharacterReference*, TurnAttack>& b) {
			CharacterStats& as = a.first->stats;
			CharacterStats& bs = b.first->stats;
			float aSpeed = as.GetStatProcessedValue(as.currentStats.speed, as.statsStages.speed);
			float bSpeed = bs.GetStatProcessedValue(bs.currentStats.speed, bs.statsStages.speed);

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
			if (attack.first->stats.currentHp <= 0)
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
				if (character.stats.currentHp <= 0)
					continue;

				for (size_t i = 0; i < character.stats.statusEffects.size(); i++)
				{
					if (character.stats.statusEffects[i].turns <= 0)
						continue;
					character.stats.statusEffects[i].turns--;
					character.stats.currentHp += character.stats.statusEffects[i].value;
				}

				character.stats.statusEffects.erase(
					std::remove_if(
						character.stats.statusEffects.begin(),
						character.stats.statusEffects.end(),
						[](const StatusEffect& c) { return c.turns <= 0; }
					),
					character.stats.statusEffects.end()
				);

				if (character.stats.currentHp < 0)
					character.stats.currentHp = 0.f;
				if (character.stats.currentHp > character.stats.currentStats.hp)
					character.stats.currentHp = (float)character.stats.currentStats.hp;


			}
		}
		CheckDeadCharacters();
		ChangeState(CombatState::END_CHECK);
		break;
	case CombatSystem::END_CHECK:
		
		for (auto& team : charactersInCombat)
		{

			bool allDead = true;

			for (auto& character : team.second)
			{
				if (character.stats.currentHp != 0)
				{
					allDead = false;
					break;
				}
			}

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
		/// End
		break;
	case CombatSystem::DEFEAT:
		/// End
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
	delete ai;
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

vector<CombatSystem::CharacterReference*> CombatSystem::GetPosibleTargets(CharacterReference* character, Attack* attack)
{
	vector<CharacterReference*> possibleTargets;

	if (attack->targetType == CombatSystem::Self) {
		possibleTargets.emplace_back(character);
		return possibleTargets;
	}

	for (auto& team : charactersInCombat)
	{
		for (auto& character : team.second)
		{
			if (character.stats.currentHp <= 0)
				continue;

			if (character.team == attack->targetType || attack->targetType == CombatSystem::Both) {
				possibleTargets.emplace_back(&character);
			}
		}
	}
	return possibleTargets;
}
