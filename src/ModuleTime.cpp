#include "ModuleTime.h"

ModuleTime::ModuleTime(bool start_active) : Module(start_active)
{
    currentTime = SDL_GetPerformanceCounter();
}

ModuleTime::~ModuleTime()
{
}

bool ModuleTime::PreUpdate()
{
    lastTime = currentTime;
    currentTime = SDL_GetPerformanceCounter();
    deltaTime = static_cast<double>(currentTime - lastTime) / SDL_GetPerformanceFrequency();
    deltaTime *= timeScale;
    return true;
}
