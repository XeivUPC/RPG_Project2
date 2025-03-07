#pragma once
#include "Module.h"
#include <string>
#include <unordered_map>

class TextureFactory;
class AtlasFactory;
class AudioFactory;
class FontFactory;
struct Vector2Int;
struct Mix_Chunk;
struct _Mix_Music;
struct _TTF_Font;
class TextureAtlas;
class MusicContainer;
class AudioContainer;

class ModuleAssetDatabase : public Module
{
	friend class Engine;
	public:
		ModuleAssetDatabase(bool start_active = true);
		~ModuleAssetDatabase();

		const TextureFactory& texture_factory();
		const AtlasFactory& atlas_factory();
		const AudioFactory& audio_factory();
		const FontFactory& font_factory();

		SDL_Texture* GetTexture(const string& textureID);
		void AddTextureToStorage(string textureID, SDL_Texture& texture);
		Vector2Int GetTextureSize(const SDL_Texture& texture) const;
		bool IsTextureLoaded(const string& textureID);
		const unordered_map<string, SDL_Texture*>& GetAllTexturesReference();
	
		Mix_Chunk* GetAudio(const string& audioID);
		AudioContainer* GetAudioContainer(const string& audioContainerID);
		_Mix_Music* GetMusic(const string& musicID);
		MusicContainer* GetMusicContainer(const string& musicContainerID);
		void AddAudioToStorage(string audioID, Mix_Chunk& audio);
		void AddAudioContainerToStorage(string audioContainerID, AudioContainer& audioContainer);
		void AddMusicToStorage(string musicID, _Mix_Music& music);
		void AddMusicContainerToStorage(string musicContainerID, MusicContainer& musicContainer);
		bool IsAudioLoaded(const string& audioID);
		bool IsAudioContainerLoaded(const string& audioContainerID);
		bool IsMusicLoaded(const string& musicID);
		bool IsMusicContainerLoaded(const string& musicContainerID);
		const unordered_map<string, Mix_Chunk*>& GetAllAudioReference();
		const unordered_map<string, AudioContainer*>& GetAllAudioContainerReference();
		const unordered_map<string, _Mix_Music*>& GetAllMusicReference();
		const unordered_map<string, MusicContainer*>& GetAllMusicContainerReference();

		TextureAtlas* GetAtlas(const string& atlasID);
		void AddAtlasToStorage(string atlasID, TextureAtlas& atlas);
		bool IsAtlasLoaded(const string& atlasID);
		const unordered_map<string, TextureAtlas*>& GetAllAtlasReference();

		_TTF_Font* GetFont(const string& fontID);
		void AddFontToStorage(string fontID, _TTF_Font& font);
		bool IsFontLoaded(const string& fontID);
		const unordered_map<string, _TTF_Font*>& GetAllFontReference();

	public:

	private:
		// Inherited via IInitializable
		bool Init() override;
		// Inherited via ICleanable
		bool CleanUp() override;

		void LoadAssets();

	private:
		unordered_map<string, SDL_Texture*> textureData;
		unordered_map<string, Mix_Chunk*> audioData;
		unordered_map<string, AudioContainer*> audioContainerData;
		unordered_map<string, _Mix_Music*> musicData;
		unordered_map<string, MusicContainer*> musicContainerData;
		unordered_map<string, TextureAtlas*> atlasData;
		unordered_map<string, _TTF_Font*> fontData;

		TextureFactory* textureFactory = nullptr;
		AtlasFactory* atlasFactory = nullptr;
		AudioFactory* audioFactory = nullptr;
		FontFactory* fontFactory = nullptr;
};