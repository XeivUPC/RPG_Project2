#pragma once
#include <vector>

struct _Mix_Music;

class MusicContainer {
    
public:
    enum class PlayMode {
        Immediate,      // Always play the first clip
        Random,         // Random clip every time
        NonRepeatingRandom,    // Cycle through all clips without repeats
        InOrder         // Play clips in sequential order
    };

    MusicContainer(PlayMode _mode, std::vector<_Mix_Music*> _tracks);
    _Mix_Music* GetNextTrack();

private:
    PlayMode mode;
    std::vector<_Mix_Music*> tracks;
    size_t lastPlayedIndex = 0;
};