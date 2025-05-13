#pragma once

class MissionCondition {
public:
	MissionCondition();
	virtual ~MissionCondition() = 0;

	virtual void SetUp() = 0;

	virtual bool Check() = 0;
	bool Completed();
	void Reset();
public:

private:

private:

protected:

protected:
	bool isDone = false;

};