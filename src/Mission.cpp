#include "Mission.h"
#include "MissionCondition.h"


Mission::Mission(string _title, string _description)
{
	title = _title;
	description = _description;
}

Mission::~Mission()
{
    for (; conditions.size() != 0;)
    {
        delete conditions[0];
        conditions.erase(conditions.begin());
    }
    conditions.clear();

}

void Mission::AddCondition(MissionCondition& condition)
{
    conditions.emplace_back(&condition);
}

bool Mission::IsCompleted()
{
    bool done = true;

    for (auto& condition : conditions) {
        done = condition->Completed();
        if (!done)
            return done;
    }

    return done;
}

Mission::State Mission::GetState()
{
    return state;
}

void Mission::SetState(State newState)
{
    state = newState;
}

const string& Mission::GetTitle()
{
	return title;
}

const string& Mission::GetDescription()
{
    return description;
}
