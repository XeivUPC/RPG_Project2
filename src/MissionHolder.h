#pragma once
#include "SystemEvent.h"
#include <vector>

using namespace std;

class MissionCondition;
class Mission;

class MissionHolder {

	


public:

	enum class State {
		IN_PROGRESS,
		COMPLETED,
		REWARDED
	};

	MissionHolder(Mission* _missionRef);
	~MissionHolder();

	void AddCondition(MissionCondition& condition);

	bool IsCompleted();

	State GetState();
	void SetState(State newState);

	const string& GetTitle();
	const string& GetDescription();
	const string& GetId();

public:
	vector<MissionCondition*> conditions;
	State state = State::IN_PROGRESS;

private:
	Mission* missionRef = nullptr;
};