#pragma once
#include "Module.h" 

#include <future>
#include <mutex>

struct _Mix_Music;
struct Mix_Chunk;

class ModuleAudio : public Module
{
	friend class Engine;
public:
	ModuleAudio(bool start_active = true);
	~ModuleAudio();

	void SetGeneralVolume(float volume);
	void SetSfxVolume(float volume);
	void SetMusicVolume(float volume);
	void SetMuteStatus(bool mute);

	void Mute();
	void UnMute();

	float GetGeneralVolume();
	float GetSFXVolume();
	float GetMusicVolume();

	void SetAudioBoost(float boost);
	float GetAudioBoost();

	void PlayMusicAsync(_Mix_Music* music, int fadeTimeMS = 0);
	int PlaySFX(Mix_Chunk* sfx, int loops = 0);
	void StopSFX(int channel);

private:
	// Inherited via IInitializable
	bool Init() override;
	// Inherited via ICleanable
	bool CleanUp() override;

	bool PlayMusic(_Mix_Music* music, int fadeTimeMS = 0);
	void StopMusic();
	float ConvertFromLinearToLogarithmic(float inputValue);

private:
	float general_volume = 1;
	float sfx_volume = 1;
	float music_volume = 1;

	float audio_boost = 1.f;

	bool isMuted = false;

	_Mix_Music* currentMusic = nullptr;
	mutex musicMutex;
	std::thread musicThread;
	std::atomic<bool> stopRequested{ false };
	std::condition_variable stopSignal;
	std::mutex stopMutex;
};