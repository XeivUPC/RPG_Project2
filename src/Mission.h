#pragma once
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class MissionCondition;

class Mission
{
public:

	struct ConditionData{
		string type;
		unordered_map<string, string> properties;
	};

	Mission();
	~Mission();

	vector<MissionCondition*> CreateConditions();

public:
	string id = "";
	string title = "";
	string description = "";

	vector<ConditionData> conditionsData;
};