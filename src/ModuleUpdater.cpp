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
	return true;
}

void ModuleUpdater::AddToUpdateQueue(IUpdateable& updateableObj, UpdateMode mode)
{
	updatesQueue[mode].emplace_back(&updateableObj);
}

void ModuleUpdater::RemoveFomUpdateQueue(IUpdateable& updateableObj, UpdateMode mode)
{
	updatesQueue[mode].erase(
		remove(updatesQueue[mode].begin(), updatesQueue[mode].end(), &updateableObj),
		updatesQueue[mode].end()
	);
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
