#include "CombatSystem.h"

class Attack
{
public:
	int priority = 0;
	virtual void DoAttack(CombatSystem::CharacterCombatStats& attacker, std::vector<CombatSystem::CharacterCombatStats&>& target);
};