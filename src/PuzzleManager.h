#pragma once

#include "SystemEvent.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class PuzzleElemetn;

class PuzzleManager {
public:

    static PuzzleManager& Instance() {
        static PuzzleManager instance;
        return instance;
    }

    void ClearData();
    void SaveAllData();
	void LoadAllData();

	bool HasPuzzle(const string& id) const;
	bool HasPuzzleProperty(const string& id, const string& propertyId) const;
    string GetValueFromPuzzle(const string& id, const string& propertyId);
    void SetValueFromPuzzle(const string& id, const string& propertyId, const string& value);

public:
    /// Ids, Paramas
	SystemEvent<string, unordered_map<string, string>> onPuzzleCall;

	SystemEvent<string> onPuzzleCompleted;

private:

    PuzzleManager(const PuzzleManager&) = delete;
    PuzzleManager& operator=(const PuzzleManager&) = delete;

    PuzzleManager();
    ~PuzzleManager();


private:
	string pathToSaveFile = "Assets/Data/Puzzles/PuzzlesData.xml";
	unordered_map<string, unordered_map<string,string>> puzzlesData;
};