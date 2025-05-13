#include "Mission.h"

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

        }
    }

    return conditions;
    
}
