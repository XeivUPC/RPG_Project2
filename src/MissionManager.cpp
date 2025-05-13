#include "MissionManager.h"
#include "Mission.h"
#include "MissionCondition.h"


void MissionManager::AddMission(Mission& mission, bool triggerEvent)
{

	missions.emplace_back(&mission);
	SetUpMission(mission);

	if (triggerEvent)
		onMissionAdded.Trigger(mission);
}

void MissionManager::RemoveMission(Mission& mission, bool triggerEvent)
{
	if(triggerEvent)
		onMissionRemoved.Trigger(mission);

	missions.erase(remove(missions.begin(), missions.end(), &mission), missions.end());
	delete &mission;
	/// remove and delete
}

MissionManager::MissionManager()
{
}

MissionManager::~MissionManager()
{

	for (; missions.size() != 0;)
	{
		delete missions[0];
		missions.erase(missions.begin());
	}
	missions.clear();
}

void MissionManager::SetUpMission(Mission& mission)
{
	//// add conditions
	for (size_t i = 0; i < mission.conditions.size(); i++)
	{
		mission.conditions[i]->SetUp();
	}
}

bool MissionManager::UpdateMissions()
{
	for (auto& mission : missions) {
		for (auto& condition : mission->conditions) {
			condition->Check();
		}

		if (mission->GetState()== Mission::State::IN_PROGRESS && mission->IsCompleted())
		{
			printf("done");
			mission->SetState(Mission::State::COMPLETED);
			onMissionCompleted.Trigger(*mission);
		}
	}
	return true;
}

