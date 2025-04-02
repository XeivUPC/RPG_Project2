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

		xml_node propertiesNode = dataNode.child("properties");
		xml_node requierementsNode = dataNode.child("requierements");
		xml_node basicStatsNode = dataNode.child("basic-stats");
		xml_node statsModificationNode = dataNode.child("stat-modification");
		xml_node statusModificationNode = dataNode.child("status-modification");
		xml_node extrasNode = dataNode.child("extras");

		newAttack.priority = propertiesNode.attribute("priority").as_int();
		newAttack.accuracy = propertiesNode.attribute("accuracy").as_int();

		newAttack.targetAmount = requierementsNode.attribute("target-amount-type").as_int();
		newAttack.targetType = (CombatSystem::CharacterType)requierementsNode.attribute("target-type").as_int();

		newAttack.power = basicStatsNode.child("power").attribute("value").as_int();
		newAttack.critRate = basicStatsNode.child("crit-rate").attribute("value").as_int();

		for (const auto& statModification : statsModificationNode.children()) {
			string statType = statModification.attribute("stat").as_string();
			Attack::StageModifier data = {  statType,
											statModification.attribute("value").as_int(),
											(CombatSystem::CharacterType)statModification.attribute("objective").as_int() };
			newAttack.statsModification[statType] = data;
		}

		for (const auto& statusModification : statusModificationNode.children()) {
			string statusType = statusModification.attribute("type").as_string();
			Attack::StatusModifier data = { statusType,
											statusModification.attribute("value").as_int(),
											statusModification.attribute("turns").as_int(),
											(CombatSystem::CharacterType)statusModification.attribute("objective").as_int() };
			newAttack.statusEffects[statusType] = data;
		}

		for (const auto& extra : extrasNode.children()) {
			string extraType = extra.attribute("type").as_string();

			if (extraType == "Live Dew") {
				newAttack.lifeDewMode = extra.attribute("mode").as_int();
				newAttack.lifeDewAmount = extra.attribute("amount").as_int();
				newAttack.lifeDewEffectiveness = extra.attribute("effectiveness").as_int();
			}
			else if (extraType == "Block Turn") {
				newAttack.blocksTurn = true;
				newAttack.blockTurnTarget = (CombatSystem::CharacterType)extra.attribute("objective").as_int();
			}
		}

		
	
		attackList.emplace_back(newAttack);
	}
}
