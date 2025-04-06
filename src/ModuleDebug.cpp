#include "ModuleDebug.h"
#include "ModuleInput.h"
#include "Engine.h"

ModuleDebug::ModuleDebug(bool start_active)
{
}

ModuleDebug::~ModuleDebug()
{
}

bool ModuleDebug::Update()
{
	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godmode = !godmode;
	}
	return true;
}
