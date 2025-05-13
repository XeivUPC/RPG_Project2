#include "MissionManager.h"
#include "MissionHolder.h"
#include "MissionCondition.h"


void MissionManager::AddMission(MissionHolder& mission, bool triggerEvent)
{

	missions.emplace_back(&mission);
	SetUpMission(mission);

	if (triggerEvent)
		onMissionAdded.Trigger(mission);
}

void MissionManager::RemoveMission(MissionHolder& mission, bool triggerEvent)
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

void MissionManager::SetUpMission(MissionHolder& mission)
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

		if (mission->GetState()== MissionHolder::State::IN_PROGRESS && mission->IsCompleted())
		{
			printf("done");
			mission->SetState(MissionHolder::State::COMPLETED);
			onMissionCompleted.Trigger(*mission);
		}
	}
	return true;
}

MissionHolder* MissionManager::GetMissionByIndex(int index)
{
	if (missions.size() == 0)
		return nullptr;
	return missions[index];
}

int MissionManager::GetMissionIndex(MissionHolder& mission)
{ 
	for (size_t i = 0; i < missions.size(); i++)
	{
		if (missions[i] == &mission)
			return i;
	}
	return -1;
}

int MissionManager::GetMissionsAmount()
{
	return missions.size();
}

