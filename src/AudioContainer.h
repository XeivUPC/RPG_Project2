
#pragma once
#include <vector>

struct Mix_Chunk;

class AudioContainer {
    
public:
    enum class PlayMode {
        Immediate,      // Always play the first clip
        Random,         // Random clip every time
        NonRepeatingRandom,    // Cycle through all clips without repeats
        InOrder         // Play clips in sequential order
    };

    AudioContainer(PlayMode _mode, std::vector<Mix_Chunk*> _clips);
    Mix_Chunk* GetNextClip();

    void AddClip(Mix_Chunk* clip);

private:
    PlayMode mode;
    std::vector<Mix_Chunk*> clips;
    size_t lastPlayedIndex = -1;
};