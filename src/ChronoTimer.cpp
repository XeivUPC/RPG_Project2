#include "ChronoTimer.h"
#include "SDL2\SDL_timer.h"
	
ChronoTimer::ChronoTimer()
{
	StartTimer();
}

ChronoTimer::~ChronoTimer()
{
}

void ChronoTimer::StartTimer()
{
	startTime = SDL_GetTicks();
}

float ChronoTimer::ReadSec() const
{
	return (SDL_GetTicks() - startTime) / 1000.f;
}

float ChronoTimer::ReadMSec() const
{
	return (float)(SDL_GetTicks() - startTime);
}
