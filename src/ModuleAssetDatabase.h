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
class Atlas;

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
		_Mix_Music* GetMusic(const string& musicID);
		void AddAudioToStorage(string audioID, Mix_Chunk& audio);
		void AddMusicToStorage(string musicID, _Mix_Music& music);
		bool IsAudioLoaded(const string& audioID);
		bool IsMusicLoaded(const string& musicID);
		const unordered_map<string, Mix_Chunk*>& GetAllAudioReference();
		const unordered_map<string, _Mix_Music*>& GetAllMusicReference();

		Atlas* GetAtlas(const string& atlasID);
		void AddAtlasToStorage(string atlasID, Atlas& atlas);
		bool IsAtlasLoaded(const string& atlasID);
		const unordered_map<string, Atlas*>& GetAllAtlasReference();

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
		unordered_map<string, _Mix_Music*> musicData;
		unordered_map<string, Atlas*> atlasData;
		unordered_map<string, _TTF_Font*> fontData;

		TextureFactory* textureFactory = nullptr;
		AtlasFactory* atlasFactory = nullptr;
		AudioFactory* audioFactory = nullptr;
		FontFactory* fontFactory = nullptr;
};