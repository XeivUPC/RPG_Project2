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
		newAttack.accuracity = dataNode.attribute("accuracity").as_int();

		newAttack.damage = dataNode.attribute("damage").as_int();
		newAttack.critRate = dataNode.attribute("cirt-rate").as_int();

		newAttack.damageIncrement = dataNode.attribute("damage-increment").as_int();
		newAttack.defenseIncrement = dataNode.attribute("defense-increment").as_int();
		newAttack.speedIncrement = dataNode.attribute("speed-increment").as_int();

		newAttack.poisonDamage = dataNode.attribute("poison-damage").as_int();
		newAttack.poisonTurns = dataNode.attribute("poison-turns").as_int();
		newAttack.burnDamage = dataNode.attribute("burn-damage").as_int();
		newAttack.burnTurns = dataNode.attribute("burn-turns").as_int();
		newAttack.regenerationValue = dataNode.attribute("regeneration-value").as_int();
		newAttack.regenerationTurns = dataNode.attribute("regeneration-turns").as_int();

		newAttack.lifeSteal = dataNode.attribute("life-steal").as_int();
		newAttack.lifeStealPercentage = dataNode.attribute("life-steal-percentage").as_int();
		newAttack.lifeStealEffectiveness = dataNode.attribute("life-steal-effectiveness").as_int();

		newAttack.blockTurn = dataNode.attribute("block-turn").as_bool();
	
		attackList.emplace_back(newAttack);
	}
}
