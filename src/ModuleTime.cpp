#include "ModuleTime.h"
#include "Engine.h"
#include "ModuleUpdater.h"

#include "Globals.h"

ModuleTime::ModuleTime(bool start_active) : Module(start_active)
{
    currentTime = SDL_GetPerformanceCounter();
}

ModuleTime::~ModuleTime()
{
 
}

bool ModuleTime::Start()
{
    Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    return true;
}

bool ModuleTime::CleanUp()
{
    Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
    return true;
}


bool ModuleTime::PreUpdate()
{
    lastTime = currentTime;
    currentTime = SDL_GetPerformanceCounter();
    deltaTime = static_cast<double>(currentTime - lastTime) / SDL_GetPerformanceFrequency();
    deltaTime *= timeScale;
    accumulator += deltaTime;

    fixedDeltaTime = 0;
    while (isgreater(accumulator, FIXED_DELTA_TIME))
    {
        accumulator -= FIXED_DELTA_TIME;
        fixedDeltaTime += FIXED_DELTA_TIME;
    }

    return true;
}

