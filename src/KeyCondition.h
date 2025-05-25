#pragma once
#include "MissionCondition.h"
#include "StepTimer.h"
#include <vector>
#include <string>

using namespace std;

class KeyCondition : public MissionCondition {
public:

	enum class KeyMethod
	{
		Press,
		Hold,
	};

	KeyCondition(vector<int> _keyId, vector<float> _timeToKeep, vector<int> mode);
	~KeyCondition() override = default;

	void SetUp() override;
	bool UpdateCondition() override;
	bool Check() override;
public:

private:

private:

	vector<int> keyIds;
	vector<float> timesToKeep;
	vector<KeyMethod> keyMethods;

	vector<StepTimer> timers;

	vector<bool> tasks;
};