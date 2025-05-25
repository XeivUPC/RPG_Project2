#pragma once

class MissionCondition {
public:
	MissionCondition();
	virtual ~MissionCondition() = 0;

	virtual void SetUp() = 0;
	virtual bool UpdateCondition();
	virtual bool Check() = 0;
	bool Completed();
	void Reset();

	void ForceComplete();
public:

private:

private:

protected:

protected:
	bool isDone = false;

	bool forceDone = false;

};