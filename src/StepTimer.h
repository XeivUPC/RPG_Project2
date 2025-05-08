#pragma once

class StepTimer
{
public:
	StepTimer();
	StepTimer(float seconds);
	~StepTimer();

	void Start();
	void Start(float seconds);
	void Step(double amount);
	float ReadSec() const;
	float ReadMSec() const;
private:
	double timePassed;
};
