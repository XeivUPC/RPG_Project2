#include "Mission.h"
#include "Inventory.h"
#include "ItemCondition.h"
#include "KeyCondition.h"



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
        int tasksAmount = conditionsData[i].amount;

        if (type == "ItemCondition") {

            vector<string> itemIds;
            vector<int> itemAmounts;
            for (size_t t = 0; t < tasksAmount; t++)
            {
                string keyHelper = "item" + to_string(t + 1);

                string itemId = conditionsData[i].properties[keyHelper + "-id"];
                int itemAmount = stoi(conditionsData[i].properties[keyHelper + "-count"]);

				itemIds.emplace_back(itemId);
				itemAmounts.emplace_back(itemAmount);
            }   
            conditions.emplace_back(new ItemCondition(itemIds, itemAmounts,nullptr));

        }
        else  if (type == "KeyCondition") {
			vector<int> keyIds;
			vector<float> keyHoldTimes;
			vector<int> keyModes;

            for (size_t t = 0; t < tasksAmount; t++)
            {
				string keyHelper = "key" +to_string(t + 1);

                int keyId = stoi(conditionsData[i].properties[keyHelper + "-id"]);
                float keyHoldTime = stof(conditionsData[i].properties[keyHelper + "-time"]);
                int keyMode = stoi(conditionsData[i].properties[keyHelper + "-mode"]);

				keyIds.emplace_back(keyId);
				keyHoldTimes.emplace_back(keyHoldTime);
				keyModes.emplace_back(keyMode);
            }     

            conditions.emplace_back(new KeyCondition(keyIds, keyHoldTimes, keyModes));
        }
    }

    return conditions;
    
}
