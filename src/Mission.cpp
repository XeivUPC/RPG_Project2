#include "Mission.h"
#include "Inventory.h"
#include "ItemCondition.h"



Mission::Mission()
{
}

Mission::~Mission()
{
}

vector<MissionCondition*> Mission::CreateConditions()
{
    vector<MissionCondition*> conditions;

    for (size_t i = 0; i < conditionsData.size(); i++)
    {
        string type = conditionsData[i].type;

        if (type == "ItemCondition") {
            string itemId = conditionsData[i].properties["item-id"];
			int itemAmount = stoi(conditionsData[i].properties["item-count"]);
            conditions.emplace_back(new ItemCondition(itemId, itemAmount,nullptr));
        }
    }

    return conditions;
    
}
