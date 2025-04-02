#include "AttackList.h"
#include "Log.h"
#include "pugixml.hpp"

using namespace pugi;
AttackList::AttackList()
{
	LoadAttacks();
}

AttackList::~AttackList()
{

	attackList.clear();
}

Attack* AttackList::GetAttack(string _name)
{
	for (size_t i = 0; i < attackList.size(); i++)
	{
		if (attackList[i].name == _name)
		{
			return &attackList[i];
		}
	}
	return nullptr;
}

Attack* AttackList::GetAttack(int ID)
{
	for (size_t i = 0; i < attackList.size(); i++)
	{
		if (attackList[i].id == ID)
		{
			return &attackList[i];
		}
	}
	return nullptr;
}

void AttackList::LoadAttacks()
{
	LOG("Loading");
	LOG("Loading attacks data from XML");
	attackList.clear();
	xml_document file;
	xml_parse_result result = file.load_file(path.c_str());
	if (result == NULL)
	{
		LOG("Attacks couldn't be loaded");
		return;
	}
	xml_node rootNode = file.child("attackList");
	for (xml_node attack : rootNode.children("attack"))
	{
		Attack newAttack;
		newAttack.name = attack.attribute("name").as_string();
		newAttack.id = attack.attribute("id").as_int();

		xml_node descriptionNode = attack.child("description");
		newAttack.description = descriptionNode.attribute("value").as_string();

		xml_node dataNode = attack.child("data");

		newAttack.targetType = (CombatSystem::CharacterType)dataNode.attribute("target-type").as_int();
		newAttack.selection = (Attack::TargetSelection)dataNode.attribute("target-selection").as_int();
		newAttack.minTargetAmmount = dataNode.attribute("min-target-amount").as_int();
		newAttack.maxTargetAmmount = dataNode.attribute("max-target-amount").as_int();

		newAttack.priority = dataNode.attribute("priority").as_int();
		newAttack.accuracy = dataNode.attribute("accuracy").as_int();

		newAttack.damage = dataNode.attribute("damage").as_int();
		newAttack.critRate = dataNode.attribute("cirt-rate").as_int();

		newAttack.damageIncrementToTarget = dataNode.attribute("damage-increment-to-target").as_int();
		newAttack.defenseIncrementToTarget = dataNode.attribute("defense-increment-to-target").as_int();
		newAttack.speedIncrementToTarget = dataNode.attribute("speed-increment-to-target").as_int();
		newAttack.damageIncrementToAttacker = dataNode.attribute("damage-increment-to-attacker").as_int();
		newAttack.defenseIncrementToAttacker = dataNode.attribute("defense-increment-to-attacker").as_int();
		newAttack.speedIncrementToAttacker = dataNode.attribute("speed-increment-to-attacker").as_int();

		newAttack.poisonDamageToTarget = dataNode.attribute("poison-damage-to-target").as_int();
		newAttack.poisonDamageToTarget = dataNode.attribute("poison-damage-to-target").as_int();
		newAttack.poisonTurnsToAttacker = dataNode.attribute("poison-turns-to-attacker").as_int();
		newAttack.poisonTurnsToAttacker = dataNode.attribute("poison-turns-to-attacker").as_int();

		newAttack.burnDamageToTarget = dataNode.attribute("burn-damage-to-target").as_int();
		newAttack.burnDamageToTarget = dataNode.attribute("burn-damage-to-target").as_int();
		newAttack.burnTurnsToAttacker = dataNode.attribute("burn-turns-to-attacker").as_int();
		newAttack.burnTurnsToAttacker = dataNode.attribute("burn-turns-to-attacker").as_int();

		newAttack.regenerationValueToTarget = dataNode.attribute("regeneration-value-to-target").as_int();
		newAttack.regenerationValueToTarget = dataNode.attribute("regeneration-value-to-target").as_int();
		newAttack.regenerationTurnsToAttacker = dataNode.attribute("regeneration-turns-to-attacker").as_int();
		newAttack.regenerationTurnsToAttacker = dataNode.attribute("regeneration-turns-to-attacker").as_int();

		newAttack.lifeSteal = dataNode.attribute("life-steal").as_int();
		newAttack.lifeStealPercentage = dataNode.attribute("life-steal-percentage").as_int();
		newAttack.lifeStealEffectiveness = dataNode.attribute("life-steal-effectiveness").as_int();

		newAttack.blockTurnToTarget = dataNode.attribute("block-turn-to-target").as_bool();
		newAttack.blockTurnToAttacker = dataNode.attribute("block-turn-to-attacker").as_bool();
	
		attackList.emplace_back(newAttack);
	}
}
