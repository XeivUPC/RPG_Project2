#include "ModuleAudio.h"
#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "Log.h"
#include "UserPrefs.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#include <algorithm>
#include <thread>


ModuleAudio::ModuleAudio(bool start_active) : Module(start_active)
{
}

ModuleAudio::~ModuleAudio()
{

}

bool ModuleAudio::Init()
{
    bool ret = true;
    SDL_Init(0);

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    {
        LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
        isActive = false;
        ret = true;
    }

    // Load support for the JPG and PNG image formats
    int flags = MIX_INIT_OGG;
    int init = Mix_Init(flags);

    if ((init & flags) != flags)
    {
        LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
        isActive = false;
        ret = true;
    }
    // Initialize SDL_mixer
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        isActive = false;
        ret = true;
    }


    SetGeneralVolume(UserPrefs::Instance().GetFloat("general_volume", 1));
    SetSfxVolume(UserPrefs::Instance().GetFloat("sfx_volume", 1));
    SetMusicVolume(UserPrefs::Instance().GetFloat("music_volume", 1));

    return ret;
}

void ModuleAudio::SetGeneralVolume(float volume)
{
    general_volume = clamp(volume, 0.0f, 1.0f);
 
    SetSfxVolume(sfx_volume);
    SetMusicVolume(music_volume);
}

void ModuleAudio::SetSfxVolume(float volume)
{
    sfx_volume = clamp(volume, 0.0f, 1.0f);
    float realVolume = ConvertFromLinearToLogarithmic(general_volume*sfx_volume*audio_boost);
    
    for (size_t i = 0; i < MIX_CHANNELS; i++)
    {
        Mix_Volume((int)i, (int)realVolume);
    }
    
    /*for (auto& chunkData : Engine::Instance().m_assetsDB->GetAllAudioReference())
    {
         Mix_VolumeChunk(chunkData.second, (int)realVolume);
    }*/
}

void ModuleAudio::SetMusicVolume(float volume)
{
    music_volume = clamp(volume, 0.0f, 1.0f);
    float realVolume = ConvertFromLinearToLogarithmic(general_volume * music_volume * audio_boost);

    Mix_VolumeMusic((int)realVolume);
}

void ModuleAudio::SetMuteStatus(bool mute)
{
    if (mute)
        Mute();
    else
        UnMute();
}

void ModuleAudio::Mute()
{
    isMuted = true;
}

void ModuleAudio::UnMute()
{
    isMuted = false;
}

float ModuleAudio::GetGeneralVolume()
{
    return general_volume;
}

float ModuleAudio::GetSFXVolume()
{
    return sfx_volume;
}

float ModuleAudio::GetMusicVolume()
{
    return music_volume;
}

void ModuleAudio::SetAudioBoost(float boost)
{
    audio_boost = boost;
    if (audio_boost < 0)
        audio_boost = 0;

    SetGeneralVolume(general_volume);
}

float ModuleAudio::GetAudioBoost()
{
    return audio_boost;
}

void ModuleAudio::PlayMusicAsync(_Mix_Music* music, int fadeTimeMS)
{
    StopMusic();

    stopRequested = false;
    musicThread = std::thread([this, music, fadeTimeMS]() {
        this->PlayMusic(music, fadeTimeMS);
        });
}

void ModuleAudio::StopMusic()
{
    {
        std::lock_guard<std::mutex> lock(stopMutex);
        stopRequested = true;
    }
    stopSignal.notify_one();

    if (musicThread.joinable()) {
        musicThread.join();
    }

    std::lock_guard<std::mutex> lock(musicMutex);

    stopRequested = false;
}

bool ModuleAudio::PlayMusic(_Mix_Music* music, int fadeTimeMS)
{
    lock_guard<mutex> lock(musicMutex);

    if (stopRequested) return false;

    bool ret = true;

    if (currentMusic != nullptr) {
        if (fadeTimeMS > 0) {
            Mix_FadeOutMusic(fadeTimeMS/2);
        }
        else {
            Mix_HaltMusic();
        }
    }

    if (fadeTimeMS > 0) {
        if (Mix_FadeInMusic(music, -1, fadeTimeMS/2) < 0) {
            ret = false;
        }
    }
    else {
        if (Mix_PlayMusic(music, -1) < 0) {
            ret = false;
        }
    }

    if (ret) {
        currentMusic = music;
    }
    return ret;
}

int ModuleAudio::PlaySFX(Mix_Chunk* sfx, int loops)
{
    if (sfx == nullptr)
        return -1;
    return Mix_PlayChannel(-1, sfx, loops);
}

void ModuleAudio::StopSFX(int channel)
{
	Mix_HaltChannel(channel);
}

float ModuleAudio::ConvertFromLinearToLogarithmic(float inputValue)
{
    float perceivedVolume = log10(1 + 9 * inputValue);
    return perceivedVolume * 128;
}



bool ModuleAudio::CleanUp()
{
    StopMusic();

    UserPrefs::Instance().SaveFloat("general_volume", general_volume);
    UserPrefs::Instance().SaveFloat("sfx_volume", sfx_volume);
    UserPrefs::Instance().SaveFloat("music_volume", music_volume);

    Mix_CloseAudio();
    Mix_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    return true;
}