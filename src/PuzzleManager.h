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

	bool AddPuzzle(PuzzleElemetn& puzzle, bool triggerEvent = true);
	bool RemovePuzzle(PuzzleElemetn& puzzle, bool triggerEvent = true);

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
};