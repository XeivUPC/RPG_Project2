#pragma once
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

class Mission;
class MissionList
{
public:
	static MissionList& Instance() {
		static MissionList instance;
		return instance;
	}
	Mission* MissionByID(string id);
private:
	string path = "Assets/Data/MissionsData.xml";
	MissionList();
	~MissionList();

	void LoadMissions();

	MissionList(const MissionList&) = delete;
	MissionList& operator=(const MissionList&) = delete;
	unordered_map<string, Mission> missionList;
};