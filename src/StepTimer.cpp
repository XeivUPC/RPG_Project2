#include "StepTimer.h"

StepTimer::StepTimer()
{
	Start();
}

StepTimer::~StepTimer()
{
}

void StepTimer::Start()
{
	timePassed = 0;
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
