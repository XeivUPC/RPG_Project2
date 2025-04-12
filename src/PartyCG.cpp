#include "PartyCG.h"
#include "Party.h"

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

PartyCG::PartyCG()
{
	party = new Party(0);

	///// AssetsLoading
	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	Mix_Chunk* btn_enter = Engine::Instance().m_assetsDB->GetAudio("btn_enter");
	Mix_Chunk* btn_click = Engine::Instance().m_assetsDB->GetAudio("btn_click");

	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("uiBox4");
	SDL_Texture* close_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex2");

	///// Elements

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*bg_texture);
	UIImage* container_image = new UIImage(*bg_texture, { 0,0 }, textureSize, { 0,0 });

	SetPosition(Vector2{ LOGIC_SCREEN_WIDTH / 2 - (float)textureSize.x / 2,LOGIC_SCREEN_HEIGHT / 2 - (float)textureSize.y / 2 });


	UITextBox* boxTitle_text = new UITextBox("Party Characters", *textFont, 32, { 184,132,78,255 }, { textureSize.x / 2 , 0 }, { textureSize.x * 2, 50 }, { 0.5f,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	boxTitle_text->SetLocalScale(0.75f);
	boxTitle_text->SetParent(container_image);


	UIButton* close_btn = new UIButton(*close_texture, { 8, 8 }, { 15,15 }, { 0,0,15,15 }, { 0,0 });
	close_btn->AddRect(UIButton::ButtonStates::HOVER, { 15,0,15,15 });
	close_btn->AddRect(UIButton::ButtonStates::PRESSED, { 30,0,15,15 });
	close_btn->onMouseClick.Subscribe([this]() {isVisible = false; });
	close_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	close_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	close_btn->SetParent(container_image);


	///// Create Slots


	AddElementToCanvas(container_image);
}

PartyCG::~PartyCG()
{
	if(party!=nullptr)
		delete party;
}