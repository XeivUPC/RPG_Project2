#pragma once

class StepTimer
{
public:
	StepTimer();
	~StepTimer();

	void Start();
	void Step(double amount);
	float ReadSec() const;
	float ReadMSec() const;
private:
	double timePassed;
};
