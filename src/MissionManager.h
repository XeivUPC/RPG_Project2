#pragma once
#include "SystemEvent.h"
#include "Vector2.h"
#include <string>
#include <vector>
#include <variant>

using namespace std;

class Mission;

class MissionManager {
public:

    static MissionManager& Instance() {
        static MissionManager instance;
        return instance;
    }


    void AddMission(Mission& mission, bool triggerEvent = true);
    void RemoveMission(Mission& mission, bool triggerEvent = true);

    // Heredado vía IUpdateable
    bool UpdateMissions();

    Mission* GetMissionByIndex(int index);
    int GetMissionIndex(Mission& mission);
    int GetMissionsAmount();


public:
    /// / Event Callbacks
    SystemEvent<unordered_map<string, std::variant<int, float, string, Vector2>>> genericEvent;

    SystemEvent<Mission&> onMissionAdded;
    SystemEvent<Mission&> onMissionCompleted;
    SystemEvent<Mission&> onMissionRemoved;

private:

    MissionManager(const MissionManager&) = delete;
    MissionManager& operator=(const MissionManager&) = delete;

    MissionManager();
    ~MissionManager();


    void SetUpMission(Mission& mission);

private:
    vector<Mission*> missions;


};
