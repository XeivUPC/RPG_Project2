#include "SaveLoadCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "GameScene.h"

#include "UITextBox.h"
#include "UIImage.h"
#include "UIButton.h"

#include "Globals.h"

SaveLoadCG::SaveLoadCG()
{
	///// AssetsLoading
	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	Mix_Chunk* btn_enter = Engine::Instance().m_assetsDB->GetAudio("btn_enter");
	Mix_Chunk* btn_click = Engine::Instance().m_assetsDB->GetAudio("btn_click");

	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("uiBox5");
	SDL_Texture* close_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex2");
	SDL_Texture* btn_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex1");

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*bg_texture);
	UIImage* container_image = new UIImage(*bg_texture, { 0,0 }, textureSize, { 0,0 });

	SetPosition(Vector2{ LOGIC_SCREEN_WIDTH / 2 - (float)textureSize.x / 2,LOGIC_SCREEN_HEIGHT / 2 - (float)textureSize.y / 2 });


	UITextBox* boxTitle_text = new UITextBox("Actions", *textFont, 32, { 184,132,78,255 }, { textureSize.x / 2 , 0 }, { textureSize.x * 2, 50 }, { 0.5f,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	boxTitle_text->SetLocalScale(0.75f);
	boxTitle_text->SetParent(container_image);

	UIButton* close_btn = new UIButton(*close_texture, { 8, 8 }, { 15,15 }, { 0,0,15,15 }, { 0,0 });
	close_btn->AddRect(UIButton::ButtonStates::HOVER, { 15,0,15,15 });
	close_btn->AddRect(UIButton::ButtonStates::PRESSED, { 30,0,15,15 });
	close_btn->onMouseClick.Subscribe([this]() {isVisible = false; });
	close_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	close_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	close_btn->SetParent(container_image);



	Vector2Int anchorPoint = { textureSize.x/2, 36 };
	float btnScale = 1;
	int btnOffset = 38;
	Vector2Int btnSize = { 86,35 };

	UIButton* save_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,0 * btnOffset }, btnSize, { 0,0,86,35 }, { 0.5f,0 });
	UITextBox* save_text = new UITextBox("Save", *textFont, 16, { 184,132,78,255 }, { 0,2 }, btnSize * btnScale, { 0.5f,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	save_text->SetParent(save_btn);
	save_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	save_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	save_btn->onMouseClick.Subscribe([this]() {Engine::Instance().s_game->SaveGameSaveData(); });
	save_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	save_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	save_btn->SetParent(container_image);



	UIButton* load_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,1 * btnOffset }, btnSize, { 0,0,86,35 }, { 0.5f,0 });
	UITextBox* load_text = new UITextBox("Load", *textFont, 16, { 184,132,78,255 }, { 0,2 }, btnSize * btnScale, { 0.5f,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	load_text->SetParent(load_btn);
	load_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	load_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	load_btn->onMouseClick.Subscribe([this]() {Engine::Instance().s_game->AskForLoadSaveData(); });
	load_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	load_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	load_btn->SetParent(container_image);




	AddElementToCanvas(container_image);
}
