#pragma once
#include "Module.h" 

#include <string>
#include <future>
#include <mutex>
#include <unordered_map>
#include <tuple>

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

	const _Mix_Music* GetMusic() const;

	void SetAudioBoost(float boost);
	float GetAudioBoost();

	void PlayMusicAsync(_Mix_Music* music, int fadeTimeMS = 0);
	int PlaySFX(Mix_Chunk* sfx, int loops = 0);
	int PlaySFXWithFade(Mix_Chunk* sfx, int channel, int loops = 0, int fadeTimeMS = 0);
	void StopSFX(int channel, int fadeTimeMS = 0);

private:
	// Inherited via IInitializable
	bool Init() override;
	// Inherited via ICleanable
	bool CleanUp() override;

	bool PlayMusic(_Mix_Music* music, int fadeTimeMS = 0);
	void StopMusic();
	float ConvertFromLinearToLogarithmic(float inputValue);

	static void ChannelFinishedCallback(int channel);
	void OnChannelFinished(int channel);

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

	std::unordered_map<int, std::tuple<Mix_Chunk*, int, int>> pendingFadeIns;
	std::mutex pendingFadeInsMutex;
};