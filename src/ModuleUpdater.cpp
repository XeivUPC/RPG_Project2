#include "ModuleUpdater.h"

ModuleUpdater::ModuleUpdater(bool start_active) : Module(start_active)
{
	priority_deleting = -1;
}

ModuleUpdater::~ModuleUpdater()
{
}

bool ModuleUpdater::PreUpdate()
{
	if(!isPaused)
		return PreUpdateAll();
	return true;
}

bool ModuleUpdater::Update()
{
	if (!isPaused)
		return UpdateAll();
	return true;
}

bool ModuleUpdater::PostUpdate()
{
	if (!isPaused)
		return PostUpdateAll();
	return true;
}

bool ModuleUpdater::CleanUp()
{
	updatesQueue.clear();
	groups.clear();
	return true;
}

void ModuleUpdater::AddToUpdateQueue(IUpdateable& updateableObj, UpdateMode mode, const string& groupId)
{
	updatesQueue[mode].emplace_back(&updateableObj);
	AddToUpdateGroup(updateableObj, groupId);
}

void ModuleUpdater::RemoveFromUpdateQueue(IUpdateable& updateableObj, UpdateMode mode, bool removeFromGroups)
{
	updatesQueue[mode].erase(
		remove(updatesQueue[mode].begin(), updatesQueue[mode].end(), &updateableObj),
		updatesQueue[mode].end()
	);
	if (removeFromGroups)
		RemoveFromUpdateGroup(updateableObj);
}

void ModuleUpdater::AddToUpdateGroup(IUpdateable& updateableObj, string groupID)
{
	if (groupID != "") {
		auto& groupVector = groups[groupID];
		auto it = find(groupVector.elements.begin(),groupVector.elements.end(),&updateableObj);

		if (it == groupVector.elements.end()) {
			groupVector.elements.emplace_back(&updateableObj);
			updateableObj.isPaused = groupVector.isPaused;
		}
	}
}

void ModuleUpdater::RemoveFromUpdateGroup(IUpdateable& updateableObj, const string& groupID)
{
	for (auto& groupEntry : groups) {
		if (groupEntry.first == groupID) {
			auto& vec = groupEntry.second.elements;
			vec.erase(remove(vec.begin(), vec.end(), &updateableObj), vec.end());
			break;
		}	
	}
}

void ModuleUpdater::RemoveFromUpdateGroup(IUpdateable& updateableObj)
{
	for (auto& groupEntry : groups) {
		auto& vec = groupEntry.second.elements;
		vec.erase(remove(vec.begin(), vec.end(), &updateableObj), vec.end());
	}
}

ModuleUpdater::UpdateGroup& ModuleUpdater::GetModifiableUpdateGroup(const string& groupID)
{
	for (auto& groupEntry : groups) {
		if (groupEntry.first == groupID) {
			return groupEntry.second;
		}
	}

	
	return groups[groupID];
}

const ModuleUpdater::UpdateGroup& ModuleUpdater::GetUpdateGroup(const string& groupID)
{
	return GetModifiableUpdateGroup(groupID);
}

void ModuleUpdater::PauseUpdateGroup(const string& groupID)
{
	UpdateGroup&  group = GetModifiableUpdateGroup(groupID);
	for (auto& entry : group.elements) {
		entry->isPaused = true;
	}
	group.isPaused = true;
}

void ModuleUpdater::ResumeUpdateGroup(const string& groupID)
{
	UpdateGroup& group = GetModifiableUpdateGroup(groupID);
	for (auto& entry : group.elements) {
		entry->isPaused = false;
	}
	group.isPaused = false;

}

void ModuleUpdater::SetStatusUpdateGroup(const string& groupID, bool status)
{
	UpdateGroup& group = GetModifiableUpdateGroup(groupID);
	for (auto& entry : group.elements) {
		entry->isPaused = status;
	}
	group.isPaused = status;
}


void ModuleUpdater::Pause()
{
	isPaused = true;
}

void ModuleUpdater::Resume()
{
	isPaused = false;
}

void ModuleUpdater::SetPauseStatus(bool status)
{
	isPaused = status;
}

bool ModuleUpdater::IsPaused()
{
	return isPaused;
}

bool ModuleUpdater::PreUpdateAll()
{
	bool ret = true;
	for (const auto& updateTask : updatesQueue[UpdateMode::PRE_UPDATE])
	{
		if(!updateTask->isPaused)
			ret = updateTask->PreUpdate();
		if (!ret)
			break;
	}
	return ret;
}

bool ModuleUpdater::UpdateAll()
{
	bool ret = true;
	for (const auto& updateTask : updatesQueue[UpdateMode::UPDATE])
	{
		if (!updateTask->isPaused)
			ret = updateTask->Update();
		if (!ret)
			break;
	}
	return ret;
}

bool ModuleUpdater::PostUpdateAll()
{
	bool ret = true;
	for (const auto& updateTask : updatesQueue[UpdateMode::POST_UPDATE])
	{
		if (!updateTask->isPaused)
			ret = updateTask->PostUpdate();
		if (!ret)
			break;
	}
	return ret;
}

