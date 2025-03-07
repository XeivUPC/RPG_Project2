#include "MusicContainer.h"

MusicContainer::MusicContainer(PlayMode _mode, std::vector<_Mix_Music*> _tracks) : mode(_mode), tracks(_tracks) {}

_Mix_Music* MusicContainer::GetNextTrack()
{
    if (tracks.empty()) return nullptr;

    switch (mode) {
    case PlayMode::Immediate:
        return tracks[0];

    case PlayMode::Random:
        return tracks[rand() % tracks.size()];

    case PlayMode::NonRepeatingRandom: {
        if (tracks.size() == 1) return tracks[0];

        size_t newIndex;
        do {
            newIndex = rand() % tracks.size();
        } while (newIndex == lastPlayedIndex);

        lastPlayedIndex = newIndex;
        return tracks[newIndex];
    }
    case PlayMode::InOrder: {
        if (lastPlayedIndex < 0)
            lastPlayedIndex = 0;
        lastPlayedIndex = (lastPlayedIndex + 1) % tracks.size();
        return tracks[lastPlayedIndex];
    }
    }
    return nullptr;
}
