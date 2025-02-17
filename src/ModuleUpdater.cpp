#include "ModuleUpdater.h"

ModuleUpdater::ModuleUpdater(bool start_active) : Module(start_active)
{
}

ModuleUpdater::~ModuleUpdater()
{
}

bool ModuleUpdater::PreUpdate()
{
	if(!isPaused)
		PreUpdateAll();
	return true;
}

bool ModuleUpdater::Update()
{
	if (!isPaused)
		UpdateAll();
	return true;
}

bool ModuleUpdater::PostUpdate()
{
	if (!isPaused)
		PostUpdateAll();
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

void ModuleUpdater::PreUpdateAll()
{
	for (const auto& updateTask : updatesQueue[UpdateMode::PRE_UPDATE])
	{
		if(!updateTask->isPaused)
			updateTask->PreUpdate();
	}
}

void ModuleUpdater::UpdateAll()
{
	for (const auto& updateTask : updatesQueue[UpdateMode::UPDATE])
	{
		if (!updateTask->isPaused)
			updateTask->Update();
	}
}

void ModuleUpdater::PostUpdateAll()
{
	for (const auto& updateTask : updatesQueue[UpdateMode::POST_UPDATE])
	{
		if (!updateTask->isPaused)
			updateTask->PostUpdate();
	}
}
