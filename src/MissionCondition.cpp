#include "MissionCondition.h"


MissionCondition::MissionCondition()
{
}

MissionCondition::~MissionCondition()
{
}

bool MissionCondition::Completed()
{
    return isDone;
}

void MissionCondition::Reset()
{
    isDone = false;
}
