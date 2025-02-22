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

	SDL_Texture* nullTexture = nullptr;
	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("uiBox1");

	SDL_Texture* toggle_texture = Engine::Instance().m_assetsDB->GetTexture("toggle_tex1");
	SDL_Texture* slider_texture = Engine::Instance().m_assetsDB->GetTexture("slider_tex1");
	SDL_Texture* btn_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex2");


	///// Background

	UIImage* bg_image = new UIImage({ 0,0 }, { LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT }, { 0,0 }, true, {0,0,LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT }, { 0,0,0,100 });
	bg_image->SetLocalScale(2);

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*bg_texture);
	UIImage* box_image = new UIImage(*bg_texture, { LOGIC_SCREEN_WIDTH / 2,LOGIC_SCREEN_HEIGHT / 2 }, textureSize, {0.5f,0.5f});
	box_image->SetLocalScale(2);


	UITextBox* boxTitle_text = new UITextBox("Settings", *textFont, 32, { 27,34,54,255 }, { LOGIC_SCREEN_WIDTH / 2 , 120 }, { textureSize.x * 2, 50 }, {0.5f,0.5f},UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	boxTitle_text->SetLocalScale(1.5f);

	UIButton* close_btn = new UIButton(*btn_texture, { LOGIC_SCREEN_WIDTH / 2 - textureSize.x, LOGIC_SCREEN_HEIGHT / 2 - textureSize.y }, { 19,19 }, { 0,0,19,19 }, {0.5f,0.5f});
	close_btn->SetLocalScale(3);
	close_btn->AddRect(UIButton::ButtonStates::HOVER, {19,0,19,19});
	close_btn->AddRect(UIButton::ButtonStates::PRESSED, {38,0,19,19});
	close_btn->onMouseClick.emplace_back([this]() {visible = false; });


	///// AudioSettings
	UITextBox* generalAudio_text = new UITextBox("General Vol.", *textFont, 32, { 27,34,54,255 }, { LOGIC_SCREEN_WIDTH / 2 , LOGIC_SCREEN_HEIGHT / 2 - 170 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UISlider* generalAudio_slider = new UISlider(*slider_texture, { LOGIC_SCREEN_WIDTH / 2,LOGIC_SCREEN_HEIGHT / 2 - 110 }, { 92,13 }, { 0,0,98,26 }, *slider_texture, { 8,26 }, { 98,0,8,26 }, { 106,0,8,26 }, { 0.5f,0.5f }, 0, 0, 1, UISlider::SliderMode::RightToLeft);
	generalAudio_slider->onValueChange.emplace_back([this](float value) {Engine::Instance().m_audio->SetGeneralVolume(value); });
	generalAudio_slider->SetValue(Engine::Instance().m_audio->GetGeneralVolume());
	generalAudio_slider->SetLocalScale(3);


	UITextBox* musicAudio_text = new UITextBox("Music Vol.", *textFont, 32, { 27,34,54,255 }, { LOGIC_SCREEN_WIDTH / 2 , LOGIC_SCREEN_HEIGHT / 2 - 40 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UISlider* musicAudio_slider = new UISlider(*slider_texture, { LOGIC_SCREEN_WIDTH/2,LOGIC_SCREEN_HEIGHT / 2 + 20}, { 92,13 }, { 0,0,98,26 }, *slider_texture, { 8,26 }, { 98,0,8,26 }, { 106,0,8,26 }, { 0.5f,0.5f }, 0, 0, 1, UISlider::SliderMode::RightToLeft);
	musicAudio_slider->onValueChange.emplace_back([this](float value) {Engine::Instance().m_audio->SetMusicVolume(value); });
	musicAudio_slider->SetValue(Engine::Instance().m_audio->GetMusicVolume());
	musicAudio_slider->SetLocalScale(3);


	UITextBox* sfxAudio_text = new UITextBox("SFX Vol.", *textFont, 32, { 27,34,54,255 }, { LOGIC_SCREEN_WIDTH / 2 , LOGIC_SCREEN_HEIGHT / 2 + 90 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UISlider* sfxAudio_slider = new UISlider(*slider_texture, { LOGIC_SCREEN_WIDTH / 2,LOGIC_SCREEN_HEIGHT / 2 + 150 }, { 92,13 }, { 0,0,98,26 }, *slider_texture, { 8,26 }, { 98,0,8,26 }, { 106,0,8,26 }, { 0.5f,0.5f }, 0, 0, 1, UISlider::SliderMode::RightToLeft);
	sfxAudio_slider->onValueChange.emplace_back([this](float value) {Engine::Instance().m_audio->SetSfxVolume(value); });
	sfxAudio_slider->SetValue(Engine::Instance().m_audio->GetSFXVolume());
	sfxAudio_slider->SetLocalScale(3);


	///// VSync Settings
	/*UIToggle* vSync_toggle = new UIToggle(*toggle_texture, { LOGIC_SCREEN_WIDTH / 2,240 }, { 19,19 }, { 0,0,19,19 }, { 19,0,19,19 }, { 0.5,0.5 }, true);
	vSync_toggle->SetLocalScale(3);*/


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
	//AddElementToCanvas(vSync_toggle);
}
