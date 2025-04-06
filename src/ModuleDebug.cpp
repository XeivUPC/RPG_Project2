#include "ModuleDebug.h"
#include "ModuleInput.h"
#include "ModuleUpdater.h"
#include "Engine.h"

ModuleDebug::ModuleDebug(bool start_active)
{
}

ModuleDebug::~ModuleDebug()
{
}

bool ModuleDebug::Start()
{
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	return true;
}



bool ModuleDebug::Update()
{
	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godmode = !godmode;
	}
	return true;
}

bool ModuleDebug::CleanUp()
{
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::UPDATE);
	return true;
}
