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
	return &attackList[ID];
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
		newAttack.description = attack.attribute("description").as_string();
		newAttack.targetType = (CombatSystem::CharacterType)attack.attribute("target-type").as_int();
		newAttack.selection = (Attack::TargetSelection)attack.attribute("target-selection").as_int();
		newAttack.minTargetAmmount = attack.attribute("min-target-amount").as_int();
		newAttack.maxTargetAmmount = attack.attribute("max-target-amount").as_int();

		newAttack.blockTurn = attack.attribute("blockTurn").as_bool();

		newAttack.priority = attack.attribute("priority").as_int();
		newAttack.accuracity = attack.attribute("accuracity").as_int();
		newAttack.damage = attack.attribute("damage").as_float();
		newAttack.damage_multiplier = attack.attribute("damage_multiplier").as_float();
		newAttack.critical_damage_multiplier = attack.attribute("critical_damage_multiplier").as_float();
		newAttack.critical_damage_percentage = attack.attribute("critical_damage_percentage").as_int();
		newAttack.defense = attack.attribute("defense").as_float();
		newAttack.extraHealth = attack.attribute("extraHealth").as_float();
		newAttack.extraHealth_multiplier = attack.attribute("extraHealth_multiplier").as_float();
		newAttack.poisonDamage = attack.attribute("poisonDamage").as_float();
		newAttack.poisonTurns = attack.attribute("poisonTurns").as_int();
		newAttack.burnDamage = attack.attribute("burnDamage").as_float();
		newAttack.burnTurns = attack.attribute("burnTurns").as_int();
		newAttack.regenerationHealth = attack.attribute("regenerationHealth").as_float();
		newAttack.regenerationTurns = attack.attribute("regenerationTurns").as_int();
		newAttack.healthStolen = attack.attribute("healthStolen").as_float();
		newAttack.healthReceived_percentage = attack.attribute("healthReceived_percentage").as_int();
	
		attackList.emplace_back(newAttack);
	}
}
