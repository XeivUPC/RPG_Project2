#include "SettingsCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"

#include "UIImage.h"
#include "UITextBox.h"
#include "UISlider.h"
#include "UIToggle.h"
#include "UIButton.h"


#include "Globals.h"

SettingsCG::SettingsCG()
{

	///// AssetsLoading
	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	Mix_Chunk* btn_enter = Engine::Instance().m_assetsDB->GetAudio("btn_enter");
	Mix_Chunk* btn_click = Engine::Instance().m_assetsDB->GetAudio("btn_click");

	SDL_Texture* nullTexture = nullptr;
	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("uiBox1");

	SDL_Texture* toggle_texture = Engine::Instance().m_assetsDB->GetTexture("toggle_tex1");
	SDL_Texture* slider_texture = Engine::Instance().m_assetsDB->GetTexture("slider_tex1");
	SDL_Texture* btn_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex2");


	///// Background

	UIImage* bg_image = new UIImage({ 0,0 }, { LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT }, { 0,0 }, true, {0,0,LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT }, { 0,0,0,100 });

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*bg_texture);
	UIImage* box_image = new UIImage(*bg_texture, { LOGIC_SCREEN_WIDTH / 2,LOGIC_SCREEN_HEIGHT / 2 }, textureSize, {0.5f,0.5f});


	UITextBox* boxTitle_text = new UITextBox("Settings", *textFont, 32, {184,132,78,255}, { LOGIC_SCREEN_WIDTH / 2 , 60 }, { textureSize.x * 2, 50 }, {0.5f,0.5f},UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	boxTitle_text->SetLocalScale(0.75f);

	UIButton* close_btn = new UIButton(*btn_texture, { LOGIC_SCREEN_WIDTH / 2 - textureSize.x/2 + (int)(15 *1) +2, LOGIC_SCREEN_HEIGHT / 2 - textureSize.y/2 + (int)(15 * 1)+2 }, { 15,15 }, { 0,0,15,15 }, {0.5f,0.5f});
	close_btn->AddRect(UIButton::ButtonStates::HOVER, {15,0,15,15});
	close_btn->AddRect(UIButton::ButtonStates::PRESSED, {30,0,15,15});
	close_btn->onMouseClick.emplace_back([this]() {isVisible = false; });
	close_btn->onMouseClick.emplace_back([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	close_btn->onMouseEnter.emplace_back([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });


	///// AudioSettings
	UITextBox* generalAudio_text = new UITextBox("General Vol.", *textFont, 16, {184,132,78,255}, { LOGIC_SCREEN_WIDTH / 2 , LOGIC_SCREEN_HEIGHT / 2 - 85 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UISlider* generalAudio_slider = new UISlider(*slider_texture, { LOGIC_SCREEN_WIDTH / 2,LOGIC_SCREEN_HEIGHT / 2 - 65 }, { 89,12 }, { 0,0,94,12 }, *slider_texture, { 5,12 }, { 94,0,5,12 }, { 99,0,5,12 }, { 0.5f,0.5f }, 0, 0, 1, UISlider::SliderMode::LeftToRight);
	generalAudio_slider->onValueChange.emplace_back([this](float value) {Engine::Instance().m_audio->SetGeneralVolume(value); });
	generalAudio_slider->SetValue(Engine::Instance().m_audio->GetGeneralVolume());



	UITextBox* musicAudio_text = new UITextBox("Music Vol.", *textFont, 16, {184,132,78,255}, { LOGIC_SCREEN_WIDTH / 2 , LOGIC_SCREEN_HEIGHT / 2 - 30 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UISlider* musicAudio_slider = new UISlider(*slider_texture, { LOGIC_SCREEN_WIDTH/2,LOGIC_SCREEN_HEIGHT / 2 - 10}, { 89,12 }, { 0,0,94,12 }, *slider_texture, { 5,12 }, { 94,0,5,12 }, { 99,0,5,12 }, { 0.5f,0.5f }, 0, 0, 1, UISlider::SliderMode::LeftToRight);
	musicAudio_slider->onValueChange.emplace_back([this](float value) {Engine::Instance().m_audio->SetMusicVolume(value); });
	musicAudio_slider->SetValue(Engine::Instance().m_audio->GetMusicVolume());



	UITextBox* sfxAudio_text = new UITextBox("SFX Vol.", *textFont, 16, {184,132,78,255}, { LOGIC_SCREEN_WIDTH / 2 , LOGIC_SCREEN_HEIGHT / 2 + 25 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UISlider* sfxAudio_slider = new UISlider(*slider_texture, { LOGIC_SCREEN_WIDTH / 2,LOGIC_SCREEN_HEIGHT / 2 + 45 }, { 89,12 }, { 0,0,94,12 }, *slider_texture, { 5,12 }, { 94,0,5,12 }, { 99,0,5,12 }, { 0.5f,0.5f }, 0, 0, 1, UISlider::SliderMode::LeftToRight);
	sfxAudio_slider->onValueChange.emplace_back([this](float value) {Engine::Instance().m_audio->SetSfxVolume(value); });
	sfxAudio_slider->SetValue(Engine::Instance().m_audio->GetSFXVolume());



	///// VSync Settings
	UITextBox* vSync_text = new UITextBox("V-Sync", *textFont, 16, { 184,132,78,255 }, { LOGIC_SCREEN_WIDTH / 2 , LOGIC_SCREEN_HEIGHT / 2 + 80 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UIToggle* vSync_toggle = new UIToggle(*toggle_texture, { LOGIC_SCREEN_WIDTH / 2, LOGIC_SCREEN_HEIGHT / 2 + 110 }, { 19,19 }, { 0,0,19,19 }, { 19,0,19,19 }, { 0.5,0.5 }, true);


	///// AddElements
	AddElementToCanvas(bg_image);
	AddElementToCanvas(box_image);
	AddElementToCanvas(close_btn);
	AddElementToCanvas(boxTitle_text);
	AddElementToCanvas(generalAudio_slider);
	AddElementToCanvas(generalAudio_text);
	AddElementToCanvas(musicAudio_slider);
	AddElementToCanvas(musicAudio_text);
	AddElementToCanvas(sfxAudio_slider);
	AddElementToCanvas(sfxAudio_text);
	AddElementToCanvas(vSync_text);
	AddElementToCanvas(vSync_toggle);
}
