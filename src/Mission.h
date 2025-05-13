#pragma once
#include "SystemEvent.h"
#include <vector>

using namespace std;

class MissionCondition;

class Mission {

	


public:

	enum class State {
		IN_PROGRESS,
		COMPLETED,
		REWARDED
	};

	Mission();
	~Mission();

	void AddCondition(MissionCondition& condition);

	bool IsCompleted();

	State GetState();
	void SetState(State newState);

public:
	vector<MissionCondition*> conditions;
	State state = State::IN_PROGRESS;
};