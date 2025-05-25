#include "MissionCondition.h"


MissionCondition::MissionCondition()
{
}

MissionCondition::~MissionCondition()
{
}

bool MissionCondition::UpdateCondition()
{
    return true;
}

bool MissionCondition::Completed()
{
    if (forceDone)
        return true;
    return isDone;
}

void MissionCondition::Reset()
{
    isDone = false;
}

void MissionCondition::ForceComplete()
{
    forceDone = true;
}
