#include "AudioFactory.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

AudioFactory::AudioFactory()
{
}

AudioFactory::~AudioFactory()
{
}

Mix_Chunk* AudioFactory::CreateAudio(const string& path) const
{
	Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
	return chunk;
}

Mix_Music* AudioFactory::CreateMusic(const string& path) const
{
	Mix_Music* chunk = Mix_LoadMUS((path).c_str());
	return chunk;
}

void AudioFactory::DeleteAudio(Mix_Chunk& audioToDelete) const
{
	Mix_FreeChunk(&audioToDelete);
}

void AudioFactory::DeleteMusic(_Mix_Music& musicToDelete) const
{
	Mix_FreeMusic(&musicToDelete);
}
