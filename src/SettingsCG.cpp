#include "SettingsCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"

#include "UIImage.h"
#include "UITextBox.h"
#include "UISlider.h"
#include "UIToggle.h"
#include "UIButton.h"


#include "Globals.h"

SettingsCG::SettingsCG(int _renderLayer)
{
	renderLayer = _renderLayer;
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

	//UIImage* bg_image = new UIImage({ 0,0 }, { LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT }, { 0,0 }, true, {0,0,LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT }, { 0,0,0,100 });

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*bg_texture);
	UIImage* box_image = new UIImage(*bg_texture, { 0,0 }, textureSize, {0,0});

	SetPosition(Vector2{ LOGIC_SCREEN_WIDTH/2 - (float)textureSize.x/2,LOGIC_SCREEN_HEIGHT/2 - (float)textureSize.y/2});



	UITextBox* boxTitle_text = new UITextBox("Settings", *textFont, 32, {184,132,78,255}, { textureSize.x / 2 , 0 }, { textureSize.x * 2, 50 }, {0.5f,0},UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	boxTitle_text->SetLocalScale(0.75f);
	boxTitle_text->SetParent(box_image);

	UIButton* close_btn = new UIButton(*btn_texture, { 8, 8 }, { 15,15 }, { 0,0,15,15 }, {0,0});
	close_btn->AddRect(UIButton::ButtonStates::HOVER, {15,0,15,15});
	close_btn->AddRect(UIButton::ButtonStates::PRESSED, {30,0,15,15});
	close_btn->onMouseClick.Subscribe([this]() {isVisible = false; });
	close_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	close_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	close_btn->SetParent(box_image);


	///// AudioSettings
	UITextBox* generalAudio_text = new UITextBox("General Vol.", *textFont, 16, {184,132,78,255}, {0 , -20 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UISlider* generalAudio_slider = new UISlider(*slider_texture, { textureSize.x / 2, 75 }, { 89,12 }, { 0,0,94,12 }, *slider_texture, { 5,12 }, { 94,0,5,12 }, { 99,0,5,12 }, { 0.5f,0.5f }, 0, 0, 1, UISlider::SliderMode::LeftToRight);

	generalAudio_text->SetParent(generalAudio_slider);
	generalAudio_slider->onValueChange.Subscribe([this](float value) {Engine::Instance().m_audio->SetGeneralVolume(value); });
	generalAudio_slider->SetValue(Engine::Instance().m_audio->GetGeneralVolume());
	generalAudio_slider->SetParent(box_image);


	UITextBox* musicAudio_text = new UITextBox("Music Vol.", *textFont, 16, {184,132,78,255}, { 0 , -20 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UISlider* musicAudio_slider = new UISlider(*slider_texture, { textureSize.x / 2, 120}, { 89,12 }, { 0,0,94,12 }, *slider_texture, { 5,12 }, { 94,0,5,12 }, { 99,0,5,12 }, { 0.5f,0.5f }, 0, 0, 1, UISlider::SliderMode::LeftToRight);
	
	musicAudio_text->SetParent(musicAudio_slider);
	musicAudio_slider->onValueChange.Subscribe([this](float value) {Engine::Instance().m_audio->SetMusicVolume(value); });
	musicAudio_slider->SetValue(Engine::Instance().m_audio->GetMusicVolume());
	musicAudio_slider->SetParent(box_image);



	UITextBox* sfxAudio_text = new UITextBox("SFX Vol.", *textFont, 16, {184,132,78,255}, { 0 , -20 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UISlider* sfxAudio_slider = new UISlider(*slider_texture, { textureSize.x / 2, 165 }, { 89,12 }, { 0,0,94,12 }, *slider_texture, { 5,12 }, { 94,0,5,12 }, { 99,0,5,12 }, { 0.5f,0.5f }, 0, 0, 1, UISlider::SliderMode::LeftToRight);
	
	sfxAudio_text->SetParent(sfxAudio_slider);
	sfxAudio_slider->onValueChange.Subscribe([this](float value) {Engine::Instance().m_audio->SetSfxVolume(value); });
	sfxAudio_slider->SetValue(Engine::Instance().m_audio->GetSFXVolume());
	sfxAudio_slider->SetParent(box_image);



	///// VSync Settings
	UITextBox* vSync_text = new UITextBox("V-Sync", *textFont, 16, { 184,132,78,255 }, { 0 , -20 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UIToggle* vSync_toggle = new UIToggle(*toggle_texture, { textureSize.x / 2, 210 }, { 19,19 }, { 19,0,19,19 }, { 0,0,19,19 }, { 0.5,0.5 }, Engine::Instance().m_render->IsVSync());
	vSync_text->SetParent(vSync_toggle);
	vSync_toggle->onValueChange.Subscribe([this](bool value) {Engine::Instance().m_render->SetVSync(value); });
	vSync_toggle->SetParent(box_image);


	///// Fullscreen Settings
	UITextBox* fullscreen_text = new UITextBox("Fullscreen", *textFont, 16, { 184,132,78,255 }, { 0 , -20 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UIToggle* fullscreen_toggle = new UIToggle(*toggle_texture, { textureSize.x / 2, 255 }, { 19,19 }, { 19,0,19,19 }, { 0,0,19,19 }, { 0.5,0.5 }, Engine::Instance().m_window->IsFullscreen());
	fullscreen_text->SetParent(fullscreen_toggle);
	fullscreen_toggle->onValueChange.Subscribe([this](bool value) {Engine::Instance().m_window->SetFullscreen(value); });
	fullscreen_toggle->SetParent(box_image);

	///// AddElements
	//AddElementToCanvas(bg_image);

	AddElementToCanvas(box_image);
}
