#include "ModuleAssetDatabase.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "TextureFactory.h"
#include "AtlasFactory.h"
#include "AudioFactory.h"
#include "FontFactory.h"
#include "Vector2Int.h"
#include "Log.h"

ModuleAssetDatabase::ModuleAssetDatabase(bool start_active) : Module(start_active)
{
}

ModuleAssetDatabase::~ModuleAssetDatabase()
{
}

bool ModuleAssetDatabase::Init()
{
	textureFactory = new TextureFactory(*Engine::Instance().m_render->renderer);
	atlasFactory = new AtlasFactory();
	audioFactory = new AudioFactory();
	fontFactory = new FontFactory();

	LoadAssets();

	return true;
}

void ModuleAssetDatabase::LoadAssets()
{

	///// LOAD ASSETS REQUIERED

	AddMusicToStorage("1", *audioFactory->CreateMusic("Assets/Sounds/Music/Level0_Music.ogg"));
	AddAudioToStorage("btn_enter", *audioFactory->CreateAudio("Assets/Sounds/SFX/btn_enter.wav"));

	AddFontToStorage("monogram", *fontFactory->CreateFont("Assets/Fonts/monogram.ttf", 12));
	AddFontToStorage("alagard", *fontFactory->CreateFont("Assets/Fonts/alagard.ttf", 12));

	AddTextureToStorage("btn_tex1", *textureFactory->CreateTexture("Assets/Textures/UI/btn.png"));
	AddTextureToStorage("toggle_tex1", *textureFactory->CreateTexture("Assets/Textures/UI/toggle.png"));
	AddTextureToStorage("slider_tex1", *textureFactory->CreateTexture("Assets/Textures/UI/slider.png"));
	AddTextureToStorage("uiBox1", *textureFactory->CreateTexture("Assets/Textures/UI/uiBox1.png"));
	AddTextureToStorage("dialogue_box", *textureFactory->CreateTexture("Assets/Textures/UI/dialogue_box.png"));
	AddTextureToStorage("dialogue_answerBox", *textureFactory->CreateTexture("Assets/Textures/UI/dialogue_answerBox.png"));
	AddTextureToStorage("team_logo", *textureFactory->CreateTexture("Assets/Textures/General/team_logo.png"));
	AddTextureToStorage("mouse_cursor", *textureFactory->CreateTexture("Assets/Textures/General/mouse_cursor.png"));

	Engine::Instance().m_audio->PlayMusicAsync(GetMusic("1"), 0);

	/////
}

const TextureFactory& ModuleAssetDatabase::texture_factory()
{
	return *textureFactory;
}

const AtlasFactory& ModuleAssetDatabase::atlas_factory()
{
	return *atlasFactory;
}

const AudioFactory& ModuleAssetDatabase::audio_factory()
{
	return *audioFactory;
}

const FontFactory& ModuleAssetDatabase::font_factory()
{
	return *fontFactory;
}

SDL_Texture* ModuleAssetDatabase::GetTexture(const string& textureID)
{
	if (!IsTextureLoaded(textureID))
	{
		printf("Texture File Not Loaded\n");
		return nullptr;
	}
	return textureData[textureID];
}

void ModuleAssetDatabase::AddTextureToStorage(string textureID, SDL_Texture& texture)
{
	textureData[textureID] = &texture;
}

Vector2Int ModuleAssetDatabase::GetTextureSize(const SDL_Texture& texture) const
{
	int width = 0;
	int height = 0;
	SDL_QueryTexture((SDL_Texture*)&texture, NULL, NULL, (int*)&width, (int*)&height);
	return Vector2Int(width, height);
}

bool ModuleAssetDatabase::IsTextureLoaded(const string& textureID)
{
	return textureData.find(textureID) != textureData.end();
}

const unordered_map<string, SDL_Texture*>& ModuleAssetDatabase::GetAllTexturesReference()
{
	return textureData;
}

Mix_Chunk* ModuleAssetDatabase::GetAudio(const string& audioID)
{
	if (!IsAudioLoaded(audioID))
	{
		printf("Audio File Not Loaded\n");
		return nullptr;
	}
	return audioData[audioID];
}

_Mix_Music* ModuleAssetDatabase::GetMusic(const string& musicID)
{
	if (!IsMusicLoaded(musicID))
	{
		printf("Audio File Not Loaded\n");
		return nullptr;
	}
	return musicData[musicID];
}

void ModuleAssetDatabase::AddAudioToStorage(string audioID, Mix_Chunk& audio)
{
	audioData[audioID] = &audio;
}

void ModuleAssetDatabase::AddMusicToStorage(string musicID, _Mix_Music& music)
{
	musicData[musicID] = &music;
}

bool ModuleAssetDatabase::IsAudioLoaded(const string& audioID)
{
	return audioData.find(audioID) != audioData.end();
}

bool ModuleAssetDatabase::IsMusicLoaded(const string& musicID)
{
	return musicData.find(musicID) != musicData.end();
}

const unordered_map<string, Mix_Chunk*>& ModuleAssetDatabase::GetAllAudioReference()
{
	return audioData;
}

const unordered_map<string, _Mix_Music*>& ModuleAssetDatabase::GetAllMusicReference()
{
	return musicData;
}

Atlas* ModuleAssetDatabase::GetAtlas(const string& atlasID)
{
	if (!IsAtlasLoaded(atlasID))
	{
		printf("Atlas File Not Loaded\n");
		return nullptr;
	}
	return atlasData[atlasID];
}

void ModuleAssetDatabase::AddAtlasToStorage(string atlasID, Atlas& atlas)
{
	atlasData[atlasID] = &atlas;
}

bool ModuleAssetDatabase::IsAtlasLoaded(const string& atlasID)
{
	return atlasData.find(atlasID) != atlasData.end();
}

const unordered_map<string, Atlas*>& ModuleAssetDatabase::GetAllAtlasReference()
{
	return atlasData;
}

_TTF_Font* ModuleAssetDatabase::GetFont(const string& fontID)
{
	if (!IsFontLoaded(fontID))
	{
		printf("Font File Not Loaded\n");
		return nullptr;
	}
	return fontData[fontID];
}

void ModuleAssetDatabase::AddFontToStorage(string fontID, _TTF_Font& font)
{
	fontData[fontID] = &font;
}

bool ModuleAssetDatabase::IsFontLoaded(const string& fontID)
{
	return fontData.find(fontID) != fontData.end();
}

const unordered_map<string, _TTF_Font*>& ModuleAssetDatabase::GetAllFontReference()
{
	return fontData;
}

bool ModuleAssetDatabase::CleanUp()
{

	LOG("Freeing textures and Image library");
	for (; textureData.size() != 0;)
	{
		textureFactory->DeleteTexture(*textureData.begin()->second);
		textureData.erase(textureData.begin()->first);
	}
	textureData.clear();

	LOG("Freeing audios");
	for (; audioData.size() != 0;)
	{
		audioFactory->DeleteAudio(*audioData.begin()->second);
		audioData.erase(audioData.begin()->first);
	}
	audioData.clear();

	LOG("Freeing music");
	for (; musicData.size() != 0;)
	{
		audioFactory->DeleteMusic(*musicData.begin()->second);
		musicData.erase(musicData.begin()->first);
	}
	musicData.clear();

	LOG("Freeing atlas");
	for (; atlasData.size() != 0;)
	{
		atlasFactory->DeleteAtlas(*atlasData.begin()->second);
		delete atlasData.begin()->second;
		atlasData.erase(atlasData.begin()->first);
	}
	atlasData.clear();

	LOG("Freeing fonts");
	for (; fontData.size() != 0;)
	{
		fontFactory->DeleteFont(*fontData.begin()->second);
		fontData.erase(fontData.begin()->first);
	}
	fontData.clear();

	delete textureFactory;
	delete atlasFactory;
	delete audioFactory;
	delete fontFactory;

	return true;
}
