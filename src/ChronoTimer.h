#pragma once

class ChronoTimer
{
public:
	ChronoTimer();
	~ChronoTimer();

	void StartTimer();
	float ReadSec() const;
	float ReadMSec() const;
private:
	int startTime;
};