#include "MissionHolder.h"
#include "Mission.h"
#include "MissionCondition.h"


MissionHolder::MissionHolder(Mission* _missionRef)
{
    missionRef = _missionRef;
	vector< MissionCondition*> conditionsTemporal = missionRef->CreateConditions();
    for(auto& conditionData : conditionsTemporal)
		AddCondition(*conditionData);
}

MissionHolder::~MissionHolder()
{
    for (; conditions.size() != 0;)
    {
        delete conditions[0];
        conditions.erase(conditions.begin());
    }
    conditions.clear();

}

void MissionHolder::AddCondition(MissionCondition& condition)
{
    conditions.emplace_back(&condition);
}

bool MissionHolder::IsCompleted()
{
    bool done = true;

    for (auto& condition : conditions) {
        done = condition->Completed();
        if (!done)
            return done;
    }

    return done;
}

MissionHolder::State MissionHolder::GetState()
{
    return state;
}

void MissionHolder::SetState(State newState)
{
    state = newState;
    if (state == State::COMPLETED)
    {
        for (auto& condition : conditions) 
			condition->ForceComplete();
    }
}

const string& MissionHolder::GetTitle()
{
	return missionRef->title;
}

const string& MissionHolder::GetDescription()
{
    return missionRef->description;
}

const string& MissionHolder::GetId()
{
    return missionRef->id;
}
