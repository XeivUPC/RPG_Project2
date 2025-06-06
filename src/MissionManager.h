#pragma once
#include "SystemEvent.h"
#include "Vector2.h"
#include <string>
#include <vector>
#include <variant>

using namespace std;

class MissionHolder;

class MissionManager {
public:

    static MissionManager& Instance() {
        static MissionManager instance;
        return instance;
    }

    void Reset();

    void AddMission(MissionHolder& mission, bool triggerEvent = true);
    void RemoveMission(MissionHolder& mission, bool triggerEvent = true);
    void RemoveMission(string missionId, bool triggerEvent = true);

    bool UpdateMissionsStatus();
    void UpdateMissions();

    bool HasMission(string missionId);

	bool IsMissionCompleted(MissionHolder& mission);
	bool IsMissionCompleted(string missionId);

    MissionHolder* GetMissionByIndex(int index);
    int GetMissionIndex(MissionHolder& mission);
    int GetMissionsAmount();
	const vector<MissionHolder*>& GetMissions() const;

	void ClearAllMissions();


public:
    /// / Event Callbacks
    SystemEvent<unordered_map<string, std::variant<int, float, string, Vector2>>> genericEvent;

    SystemEvent<MissionHolder&> onMissionAdded;
    SystemEvent<MissionHolder&> onMissionCompleted;
    SystemEvent<MissionHolder&> onMissionRemoved;

private:

    MissionManager(const MissionManager&) = delete;
    MissionManager& operator=(const MissionManager&) = delete;

    MissionManager();
    ~MissionManager();


    void SetUpMission(MissionHolder& mission);

private:
    vector<MissionHolder*> missions;


};
