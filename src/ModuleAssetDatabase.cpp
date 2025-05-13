#include "ModuleAssetDatabase.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "TextureFactory.h"
#include "AtlasFactory.h"
#include "AudioFactory.h"
#include "FontFactory.h"
#include "Vector2Int.h"
#include "AudioContainer.h"
#include "MusicContainer.h"
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

//// Sounds //pepe
	////Music
	AddMusicToStorage("mainTheme", *audioFactory->CreateMusic("Assets/Sounds/Music/MainMenu.ogg"));
	AddMusicToStorage("townTheme", *audioFactory->CreateMusic("Assets/Sounds/Music/TownTheme.ogg"));
	
	AddMusicToStorage("meetingArtis", *audioFactory->CreateMusic("Assets/Sounds/Music/ArtisDance.ogg"));
	AddMusicToStorage("deathOfEmperor", *audioFactory->CreateMusic("Assets/Sounds/Music/DeathOfAnEmperor.ogg"));

	AddMusicToStorage("zaliumAnthemOrchestra", *audioFactory->CreateMusic("Assets/Sounds/Music/ZaliumAnthemOrchestra.ogg"));
	AddMusicToStorage("herosMarch", *audioFactory->CreateMusic("Assets/Sounds/Music/HerosMarch.ogg"));

	///SFX
	AddAudioToStorage("alert", *audioFactory->CreateAudio("Assets/Sounds/SFX/alert.wav"));
	AddAudioToStorage("logo_intro", *audioFactory->CreateAudio("Assets/Sounds/SFX/logo_intro.wav"));
	AddAudioToStorage("btn_enter", *audioFactory->CreateAudio("Assets/Sounds/SFX/btn_enter.wav"));
	AddAudioToStorage("btn_click", *audioFactory->CreateAudio("Assets/Sounds/SFX/btn_click1.wav"));
	AddAudioToStorage("btn_click2", *audioFactory->CreateAudio("Assets/Sounds/SFX/btn_click2.wav"));
	AddAudioToStorage("btn_click3", *audioFactory->CreateAudio("Assets/Sounds/SFX/btn_click3.wav"));
	AddAudioToStorage("ambient_birds1", *audioFactory->CreateAudio("Assets/Sounds/SFX/Ambient/forest_birds1.wav"));
	AddAudioToStorage("ambient_birds2", *audioFactory->CreateAudio("Assets/Sounds/SFX/Ambient/forest_birds2.wav"));
	AddAudioToStorage("rain_sfx", *audioFactory->CreateAudio("Assets/Sounds/SFX/Ambient/rain.wav"));
	AddAudioToStorage("enter_tent", *audioFactory->CreateAudio("Assets/Sounds/SFX/enter_tent.wav"));
	AddAudioToStorage("open_door", *audioFactory->CreateAudio("Assets/Sounds/SFX/open_door.wav"));
	AddAudioToStorage("close_door", *audioFactory->CreateAudio("Assets/Sounds/SFX/close_door.wav"));
	AddAudioToStorage("change_area", *audioFactory->CreateAudio("Assets/Sounds/SFX/change_area.wav"));

	AudioContainer* footstepContainer = new AudioContainer(AudioContainer::PlayMode::NonRepeatingRandom, { });
	AddAudioContainerToStorage("footsteps_container", *footstepContainer);
	for (size_t i = 1; i <= 12; i++)
	{
		Mix_Chunk* audioFile = audioFactory->CreateAudio("Assets/Sounds/SFX/Footsteps/footStep" + to_string(i) + ".wav");
		AddAudioToStorage("footstep" + to_string(i), *audioFile);
		footstepContainer->AddClip(audioFile);
	}

	AddAudioContainerToStorage("birds_container", *new AudioContainer(AudioContainer::PlayMode::NonRepeatingRandom, {GetAudio("ambient_birds1"),GetAudio("ambient_birds2") }));

//// Fonts
	AddFontToStorage("monogram", *fontFactory->CreateFont("Assets/Fonts/monogram.ttf", 12));
	AddFontToStorage("alagard", *fontFactory->CreateFont("Assets/Fonts/alagard.ttf", 12));

//// Textures
	///Menus
	
	AddTextureToStorage("battle_bg", *textureFactory->CreateTexture("Assets/Textures/UI/battle_bg.png"));
	AddTextureToStorage("arrow_tex1", *textureFactory->CreateTexture("Assets/Textures/UI/arrow1.png"));
	AddTextureToStorage("arrow_tex2", *textureFactory->CreateTexture("Assets/Textures/UI/arrow2.png"));
	AddTextureToStorage("tick_tex1", *textureFactory->CreateTexture("Assets/Textures/UI/tick1.png"));
	AddTextureToStorage("btn_tex1", *textureFactory->CreateTexture("Assets/Textures/UI/btn1.png"));
	AddTextureToStorage("btn_tex2", *textureFactory->CreateTexture("Assets/Textures/UI/btn2.png"));
	AddTextureToStorage("btn_tex3", *textureFactory->CreateTexture("Assets/Textures/UI/btn3.png"));
	AddTextureToStorage("btn_tex4", *textureFactory->CreateTexture("Assets/Textures/UI/btn4.png"));
	AddTextureToStorage("btn_tex5", *textureFactory->CreateTexture("Assets/Textures/UI/btn5.png"));
	AddTextureToStorage("btn_tex6", *textureFactory->CreateTexture("Assets/Textures/UI/btn6.png"));
	AddTextureToStorage("toggle_tex1", *textureFactory->CreateTexture("Assets/Textures/UI/toggle.png"));
	AddTextureToStorage("toggle_tex2", *textureFactory->CreateTexture("Assets/Textures/UI/toggle2.png"));
	AddTextureToStorage("effects_toggle", *textureFactory->CreateTexture("Assets/Textures/UI/effectsToggle.png"));
	AddTextureToStorage("slider_tex1", *textureFactory->CreateTexture("Assets/Textures/UI/slider.png"));
	AddTextureToStorage("uiBox1", *textureFactory->CreateTexture("Assets/Textures/UI/uiBox1.png"));
	AddTextureToStorage("uiBox2", *textureFactory->CreateTexture("Assets/Textures/UI/uiBox2.png"));
	AddTextureToStorage("uiBox3", *textureFactory->CreateTexture("Assets/Textures/UI/uiBox3.png"));
	AddTextureToStorage("uiBox4", *textureFactory->CreateTexture("Assets/Textures/UI/uiBox4.png"));
	AddTextureToStorage("uiBox5", *textureFactory->CreateTexture("Assets/Textures/UI/uiBox5.png"));
	AddTextureToStorage("uiBox6", *textureFactory->CreateTexture("Assets/Textures/UI/uiBox6.png"));
	AddTextureToStorage("dialogue_box", *textureFactory->CreateTexture("Assets/Textures/UI/dialogue_box.png"));
	AddTextureToStorage("combat_characterUI", *textureFactory->CreateTexture("Assets/Textures/UI/combatCharacterUI.png"));
	AddTextureToStorage("party_characterUI", *textureFactory->CreateTexture("Assets/Textures/UI/partyCharacterUI.png"));
	AddTextureToStorage("member_characterUI", *textureFactory->CreateTexture("Assets/Textures/UI/memberCharacterUI.png"));
	AddTextureToStorage("dialogue_answerBox", *textureFactory->CreateTexture("Assets/Textures/UI/dialogue_answerBox.png"));
	AddTextureToStorage("compass_overlay", *textureFactory->CreateTexture("Assets/Textures/UI/compass_overlay.png"));
	AddTextureToStorage("compass_arrow", *textureFactory->CreateTexture("Assets/Textures/UI/compass_arrow.png"));
	AddTextureToStorage("item_slot", *textureFactory->CreateTexture("Assets/Textures/UI/item_slot.png"));
	AddTextureToStorage("missions_layOut", *textureFactory->CreateTexture("Assets/Textures/UI/missions_layOut.png"));

	/// Parallax
	AddTextureToStorage("title_bg1", *textureFactory->CreateTexture("Assets/Textures/Parallax/TitleBG/title_bg1.png"));
	AddTextureToStorage("title_bg2", *textureFactory->CreateTexture("Assets/Textures/Parallax/TitleBG/title_bg2.png"));
	AddTextureToStorage("title_bg3", *textureFactory->CreateTexture("Assets/Textures/Parallax/TitleBG/title_bg3.png"));
	AddTextureToStorage("title_bg4", *textureFactory->CreateTexture("Assets/Textures/Parallax/TitleBG/title_bg4.png"));
	AddTextureToStorage("title_bg5", *textureFactory->CreateTexture("Assets/Textures/Parallax/TitleBG/title_bg5.png"));
	AddTextureToStorage("title_bg6", *textureFactory->CreateTexture("Assets/Textures/Parallax/TitleBG/title_bg6.png"));
	///Cursors
	AddTextureToStorage("mouse_cursor0", *textureFactory->CreateTexture("Assets/Textures/Cursors/mouse_cursor_0.png"));
	AddTextureToStorage("mouse_cursor1", *textureFactory->CreateTexture("Assets/Textures/Cursors/mouse_cursor_1.png"));
	AddTextureToStorage("mouse_cursor2", *textureFactory->CreateTexture("Assets/Textures/Cursors/mouse_cursor_2.png"));
	AddTextureToStorage("mouse_cursor3", *textureFactory->CreateTexture("Assets/Textures/Cursors/mouse_cursor_3.png"));
	///Icons
	AddTextureToStorage("icons_16", *textureFactory->CreateTexture("Assets/Textures/Icon/icons_16.png"));
	AddTextureToStorage("icons_8", *textureFactory->CreateTexture("Assets/Textures/Icon/icons_8.png"));
	///General
	AddTextureToStorage("team_logo", *textureFactory->CreateTexture("Assets/Textures/General/team_logo.png"));
	AddTextureToStorage("game_title_pixel", *textureFactory->CreateTexture("Assets/Textures/General/game_title_pixel.png"));
	AddTextureToStorage("game_title_&_logo", *textureFactory->CreateTexture("Assets/Textures/General/game_title_&_logo.png"));
	AddTextureToStorage("top_fade", *textureFactory->CreateTexture("Assets/Textures/General/top_fade.png"));
	AddTextureToStorage("vignette", *textureFactory->CreateTexture("Assets/Textures/General/vignette.png"));
	AddTextureToStorage("rain_effect", *textureFactory->CreateTexture("Assets/Textures/General/rain_effect_spritesheet.png"));
	///Characters --> Make Atlas later
	AddTextureToStorage("character_atlas", *textureFactory->CreateTexture("Assets/Textures/Atlas/Characters/CharactersAtlas.png"));
	AddAtlasToStorage("character_atlas",*atlasFactory->CreateAtlas(*GetTexture("character_atlas"), "Assets/Textures/Atlas/Characters/CharactersAtlas.xml"));

	///Items --> 
	AddTextureToStorage("items_atlas", *textureFactory->CreateTexture("Assets/Textures/Atlas/Items/ItemsAtlas.png"));
	AddAtlasToStorage("items_atlas", *atlasFactory->CreateAtlas(*GetTexture("items_atlas"), "Assets/Textures/Atlas/Items/ItemsAtlas.xml"));

	AddTextureToStorage("pj_test", *textureFactory->CreateTexture("Assets/Textures/Characters/player_spritesheet.png"));
	AddTextureToStorage("guard_test", *textureFactory->CreateTexture("Assets/Textures/Characters/guard_spritesheet.png"));
	AddTextureToStorage("npc_test", *textureFactory->CreateTexture("Assets/Textures/Characters/npc_spritesheet.png"));
	AddTextureToStorage("artis_test", *textureFactory->CreateTexture("Assets/Textures/Characters/artis_spritesheet.png"));
	AddTextureToStorage("cassian_test", *textureFactory->CreateTexture("Assets/Textures/Characters/cassian_spritesheet.png"));
	AddTextureToStorage("zeryn_test", *textureFactory->CreateTexture("Assets/Textures/Characters/zeryn_spritesheet.png"));
	///Tilesets
	AddTextureToStorage("floors_tiles", *textureFactory->CreateTexture("Assets/Textures/Tilesets/Floors_Tiles.png"));
	AddTextureToStorage("water_tiles", *textureFactory->CreateTexture("Assets/Textures/Tilesets/Water_Tiles.png"));
	AddTextureToStorage("cave_tiles", *textureFactory->CreateTexture("Assets/Textures/Tilesets/Cave_Tiles.png"));
	AddTextureToStorage("grass_tiles", *textureFactory->CreateTexture("Assets/Textures/Tilesets/Grass_Tiles.png"));

	AddTextureToStorage("building_atlas", *textureFactory->CreateTexture("Assets/Textures/Atlas/MapObjects/Buildings/BuildingsAtlas.png"));
	AddAtlasToStorage("building_atlas", *atlasFactory->CreateAtlas(*GetTexture("building_atlas"), "Assets/Textures/Atlas/MapObjects/Buildings/BuildingsAtlas.xml"));

	AddTextureToStorage("vegetation_atlas", *textureFactory->CreateTexture("Assets/Textures/Atlas/MapObjects/Vegetation/VegetationAtlas.png"));
	AddAtlasToStorage("vegetation_atlas", *atlasFactory->CreateAtlas(*GetTexture("vegetation_atlas"), "Assets/Textures/Atlas/MapObjects/Vegetation/VegetationAtlas.xml"));

	AddTextureToStorage("furniture_atlas", *textureFactory->CreateTexture("Assets/Textures/Atlas/MapObjects/Furniture/FurnitureAtlas.png"));
	AddAtlasToStorage("furniture_atlas", *atlasFactory->CreateAtlas(*GetTexture("furniture_atlas"), "Assets/Textures/Atlas/MapObjects/Furniture/FurnitureAtlas.xml"));

	

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

AudioContainer* ModuleAssetDatabase::GetAudioContainer(const string& audioContainerID)
{
	if (!IsAudioContainerLoaded(audioContainerID))
	{
		printf("Audio Container File Not Loaded\n");
		return nullptr;
	}
	return audioContainerData[audioContainerID];
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

MusicContainer* ModuleAssetDatabase::GetMusicContainer(const string& musicContainerID)
{
	if (!IsMusicContainerLoaded(musicContainerID))
	{
		printf("Music Container File Not Loaded\n");
		return nullptr;
	}
	return musicContainerData[musicContainerID];
}

void ModuleAssetDatabase::AddAudioToStorage(string audioID, Mix_Chunk& audio)
{
	audioData[audioID] = &audio;
}

void ModuleAssetDatabase::AddAudioContainerToStorage(string audioContainerID, AudioContainer& audioContainer)
{
	audioContainerData[audioContainerID] = &audioContainer;
}

void ModuleAssetDatabase::AddMusicToStorage(string musicID, _Mix_Music& music)
{
	musicData[musicID] = &music;
}

void ModuleAssetDatabase::AddMusicContainerToStorage(string musicContainerID, MusicContainer& musicContainer)
{
	musicContainerData[musicContainerID] = &musicContainer;
}

bool ModuleAssetDatabase::IsAudioLoaded(const string& audioID)
{
	return audioData.find(audioID) != audioData.end();
}

bool ModuleAssetDatabase::IsAudioContainerLoaded(const string& audioContainerID)
{
	return audioContainerData.find(audioContainerID) != audioContainerData.end();
}

bool ModuleAssetDatabase::IsMusicLoaded(const string& musicID)
{
	return musicData.find(musicID) != musicData.end();
}

bool ModuleAssetDatabase::IsMusicContainerLoaded(const string& musicContainerID)
{
	return musicContainerData.find(musicContainerID) != musicContainerData.end();
}

const unordered_map<string, Mix_Chunk*>& ModuleAssetDatabase::GetAllAudioReference()
{
	return audioData;
}

const unordered_map<string, AudioContainer*>& ModuleAssetDatabase::GetAllAudioContainerReference()
{
	return audioContainerData;
}

const unordered_map<string, _Mix_Music*>& ModuleAssetDatabase::GetAllMusicReference()
{
	return musicData;
}

const unordered_map<string, MusicContainer*>& ModuleAssetDatabase::GetAllMusicContainerReference()
{
	return musicContainerData;
}

TextureAtlas* ModuleAssetDatabase::GetAtlas(const string& atlasID)
{
	if (!IsAtlasLoaded(atlasID))
	{
		printf("Atlas File Not Loaded\n");
		return nullptr;
	}
	return atlasData[atlasID];
}

void ModuleAssetDatabase::AddAtlasToStorage(string atlasID, TextureAtlas& atlas)
{
	atlasData[atlasID] = &atlas;
}

bool ModuleAssetDatabase::IsAtlasLoaded(const string& atlasID)
{
	return atlasData.find(atlasID) != atlasData.end();
}

const unordered_map<string, TextureAtlas*>& ModuleAssetDatabase::GetAllAtlasReference()
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
	for (auto& [id, container] : audioContainerData) {
		delete container;
	}
	audioContainerData.clear();

	LOG("Freeing music");
	for (; musicData.size() != 0;)
	{
		audioFactory->DeleteMusic(*musicData.begin()->second);
		musicData.erase(musicData.begin()->first);
	}
	musicData.clear();
	for (auto& [id, container] : musicContainerData) {
		delete container;
	}
	musicContainerData.clear();

	LOG("Freeing atlas");
	for (; atlasData.size() != 0;)
	{
		atlasFactory->DeleteAtlas(*atlasData.begin()->second);
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
