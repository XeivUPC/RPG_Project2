#include "StepTimer.h"

StepTimer::StepTimer()
{
	Start();
}

StepTimer::StepTimer(float seconds)
{
	timePassed = seconds;
}

StepTimer::~StepTimer()
{
}

void StepTimer::Start()
{
	timePassed = 0;
}

void StepTimer::Start(float seconds)
{
	timePassed = seconds;
}

void StepTimer::Step(double amount)
{
	timePassed += amount;
}

float StepTimer::ReadSec() const
{
	return (float)timePassed;
}

float StepTimer::ReadMSec() const
{
	return (float)(timePassed * 1000);
}
