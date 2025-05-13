#include "MissionList.h"
#include "Log.h"
#include "pugixml.hpp"
#include "Mission.h"

using namespace pugi;

Mission* MissionList::MissionByID(string id)
{
	if (missionList.count(id) == 0)
		return nullptr;
    return &missionList[id];
}

MissionList::MissionList()
{
    LoadMissions();
}

MissionList::~MissionList()
{
    missionList.clear();
}

void MissionList::LoadMissions()
{
	LOG("Loading");
	LOG("Loading missions data from XML");
	missionList.clear();
	xml_document file;
	xml_parse_result result = file.load_file(path.c_str());
	if (result == NULL)
	{
		LOG("Missions couldn't be loaded");
		return;
	}
	xml_node rootNode = file.child("missionsList");
	for (xml_node mission_node : rootNode.children("mission"))
	{
		Mission newItem = Mission();
		newItem.id = mission_node.attribute("id").as_string();
		newItem.title = mission_node.child("title").text().as_string();
		newItem.description = mission_node.child("description").text().as_string();
		
		xml_node conditionsNode = mission_node.child("conditions");
		for (xml_node condition : conditionsNode.children())
		{
			Mission::ConditionData newConditionData;
			string type = condition.attribute("type").as_string();
			newConditionData.type = type;

			for (xml_node conditionData : condition.children())
			{
				string dataName = conditionData.name();
				for (xml_attribute conditionDataAttribute : conditionData.attributes())
				{
					string attributeName = conditionDataAttribute.name();
					string attributeValue = conditionDataAttribute.as_string();

					newConditionData.properties[dataName+ "-" + attributeName] = attributeValue;
				}
			}
			newItem.conditionsData.emplace_back(newConditionData);
		}
		missionList.emplace(newItem.id, newItem);
	}
}
