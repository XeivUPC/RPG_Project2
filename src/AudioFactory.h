#pragma once
#include "ModuleAssetDatabase.h"
#include <string>

struct Mix_Chunk;
struct _Mix_Music;

class AudioFactory
{
public:
	AudioFactory();
	~AudioFactory();

	Mix_Chunk* CreateAudio(const string& path) const;
	_Mix_Music* CreateMusic(const string& path) const;
	void DeleteAudio(Mix_Chunk& audioToDelete) const;
	void DeleteMusic(_Mix_Music& musicToDelete) const;
public:

private:

private:

};