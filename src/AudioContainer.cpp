#include "AudioContainer.h"

AudioContainer::AudioContainer(PlayMode _mode, std::vector<Mix_Chunk*> _clips)
    : mode(_mode), clips(_clips) {}

Mix_Chunk* AudioContainer::GetNextClip() {
    if (clips.empty()) return nullptr;

    switch (mode) {
    case PlayMode::Immediate:
        return clips[0];

    case PlayMode::Random:
        return clips[rand() % clips.size()];

    case PlayMode::NonRepeatingRandom: {
        if (clips.size() == 1) return clips[0];

        size_t newIndex;
        do {
            newIndex = rand() % clips.size();
        } while (newIndex == lastPlayedIndex);

        lastPlayedIndex = newIndex;
        return clips[newIndex];
    }
    case PlayMode::InOrder: {
        if (lastPlayedIndex < 0)
            lastPlayedIndex = 0;
        lastPlayedIndex = (lastPlayedIndex + 1) % clips.size();
        return clips[lastPlayedIndex];
    }
    }
    return nullptr;
}

void AudioContainer::AddClip(Mix_Chunk* clip)
{
    clips.emplace_back(clip);
}
