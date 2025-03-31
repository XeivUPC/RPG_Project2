#include "CombatSystem.h"

class Attack
{
public:
	string name = "Default";
	string description;
	int priority = 0;
	virtual void DoAttack(CombatSystem::CharacterReference& attacker, std::vector<CombatSystem::CharacterReference*>& target);
};