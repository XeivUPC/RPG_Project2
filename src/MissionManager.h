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


    void AddMission(MissionHolder& mission, bool triggerEvent = true);
    void RemoveMission(MissionHolder& mission, bool triggerEvent = true);

    // Heredado vía IUpdateable
    bool UpdateMissions();

    MissionHolder* GetMissionByIndex(int index);
    int GetMissionIndex(MissionHolder& mission);
    int GetMissionsAmount();


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
