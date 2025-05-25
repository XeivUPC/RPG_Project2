#include "MissionManager.h"
#include "MissionHolder.h"
#include "MissionCondition.h"


void MissionManager::Reset()
{
	onMissionAdded.UnsubscribeAll();
	onMissionRemoved.UnsubscribeAll();
	onMissionCompleted.UnsubscribeAll();
	genericEvent.UnsubscribeAll();
}

void MissionManager::AddMission(MissionHolder& mission, bool triggerEvent)
{

	if (HasMission(mission.GetId()))
	{
		delete& mission;
		return;
	}
	missions.emplace_back(&mission);
	SetUpMission(mission);


	if (triggerEvent)
		onMissionAdded.Trigger(mission);

	UpdateMissionsStatus();

	if(mission.GetState() == MissionHolder::State::COMPLETED)
		onMissionCompleted.Trigger(mission);

}

void MissionManager::RemoveMission(MissionHolder& mission, bool triggerEvent)
{

	missions.erase(remove(missions.begin(), missions.end(), &mission), missions.end());

	if (triggerEvent)
		onMissionRemoved.Trigger(mission);

	delete &mission;
}

void MissionManager::RemoveMission(string missionId, bool triggerEvent)
{
	for (auto& mission : missions) {
		if (mission->GetId() == missionId)
			RemoveMission(*mission, triggerEvent);
	}
}

MissionManager::MissionManager()
{
}

MissionManager::~MissionManager()
{
	Reset();
	ClearAllMissions();
}

void MissionManager::SetUpMission(MissionHolder& mission)
{
	//// add conditions
	for (size_t i = 0; i < mission.conditions.size(); i++)
	{
		mission.conditions[i]->SetUp();
	}
}

bool MissionManager::UpdateMissionsStatus()
{
	for (auto& mission : missions) {
		for (auto& condition : mission->conditions) {
			condition->Check();
		}

		bool isCompleted = mission->IsCompleted();
		MissionHolder::State state = mission->GetState();

		if (state == MissionHolder::State::IN_PROGRESS && isCompleted)
		{
			mission->SetState(MissionHolder::State::COMPLETED);
			onMissionCompleted.Trigger(*mission);
		}
		else if(state == MissionHolder::State::COMPLETED && !isCompleted){
			mission->SetState(MissionHolder::State::IN_PROGRESS);
		}
	}
	return true;
}

void MissionManager::UpdateMissions()
{
	for (auto& mission : missions) {
		for (auto& condition : mission->conditions) {
			condition->UpdateCondition();
		}
	}
}

bool MissionManager::HasMission(string missionId)
{
	for (auto& mission : missions) {
		if (mission->GetId() == missionId)
			return true;
	}
	return false;
}

bool MissionManager::IsMissionCompleted(MissionHolder& mission)
{
	return (mission.GetState() == MissionHolder::State::COMPLETED);
}

bool MissionManager::IsMissionCompleted(string missionId)
{
	for (auto& mission : missions) {
		if(mission->GetId() == missionId)
			return IsMissionCompleted(*mission);
	}
	return false;
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
			return (int)i;
	}
	return -1;
}

int MissionManager::GetMissionsAmount()
{
	return (int)missions.size();
}

const vector<MissionHolder*>& MissionManager::GetMissions() const
{
	return missions;
}

void MissionManager::ClearAllMissions()
{
	for (; missions.size() != 0;)
	{
		delete missions[0];
		missions.erase(missions.begin());
	}
	missions.clear();
}

