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

	Mission(string _title, string _description);
	~Mission();

	void AddCondition(MissionCondition& condition);

	bool IsCompleted();

	State GetState();
	void SetState(State newState);

	const string& GetTitle();
	const string& GetDescription();

public:
	vector<MissionCondition*> conditions;
	State state = State::IN_PROGRESS;

private:
	string title="Title";
	string description = "Description";
};