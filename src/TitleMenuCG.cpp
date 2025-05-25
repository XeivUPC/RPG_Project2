#include "TitleMenuCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleCursor.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "TitleScene.h"

#include "UIButton.h"
#include "UITextBox.h"

#include "Globals.h"


TitleMenuCG::TitleMenuCG(UICanvas& _settingsReference) : settingsReference(&_settingsReference)
{
	///// AssetsLoading
	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	Mix_Chunk* btn_enter = Engine::Instance().m_assetsDB->GetAudio("btn_enter");
	Mix_Chunk* btn_click = Engine::Instance().m_assetsDB->GetAudio("btn_click");

	SDL_Texture* btn_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex1");



	///// Buttons
	Vector2Int btnSize = { 86,35 };
	Vector2Int anchorPoint = { LOGIC_SCREEN_WIDTH / 2+220, LOGIC_SCREEN_HEIGHT / 2 };
	float btnScale = 1;

	UITextBox* startNewGame_text = new UITextBox("New Game", *textFont, 16, { 184,132,78,255 }, anchorPoint + Vector2Int{ 2,-75 + (int)btnScale }, btnSize* btnScale, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UIButton* startNewGame_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{0,-75}, btnSize, { 0,0,86,35 }, { 0.5f,0.5f });
	startNewGame_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	startNewGame_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	startNewGame_btn->onMouseClick.Subscribe([this]() {Engine::Instance().s_title->StartGame(true); });
	startNewGame_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	startNewGame_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	startNewGame_btn->onMouseEnter.Subscribe([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	startNewGame_btn->onMouseExit.Subscribe([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });
	startNewGame_btn->SetLocalScale(btnScale);
	

	UITextBox* continueGame_text = new UITextBox("Continue", *textFont, 16, { 184,132,78,255 }, anchorPoint + Vector2Int{ 2,-25 + (int)btnScale }, btnSize * btnScale, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UIButton* continueGame_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,-25 }, btnSize, { 0,0,86,35 }, { 0.5f,0.5f });
	continueGame_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	continueGame_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	continueGame_btn->onMouseClick.Subscribe([this]() {Engine::Instance().s_title->StartGame(false); });
	continueGame_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	continueGame_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	continueGame_btn->onMouseEnter.Subscribe([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	continueGame_btn->onMouseExit.Subscribe([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });
	continueGame_btn->SetLocalScale(btnScale);


	UITextBox* settings_text = new UITextBox("Settings", *textFont, 16, { 184,132,78,255 }, anchorPoint + Vector2Int{ 2,25 + (int)btnScale }, btnSize * btnScale, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UIButton* settings_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,25 }, btnSize, { 0,0,86,35 }, { 0.5f,0.5f });
	settings_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	settings_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	settings_btn->onMouseClick.Subscribe([this]() {settingsReference->isVisible = true; });
	settings_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	settings_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	settings_btn->onMouseEnter.Subscribe([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	settings_btn->onMouseExit.Subscribe([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });
	settings_btn->SetLocalScale(btnScale);


	UITextBox* exit_text = new UITextBox("Exit", *textFont, 16, { 184,132,78,255 }, anchorPoint + Vector2Int{ 2,75 + (int)btnScale }, btnSize * btnScale, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UIButton* exit_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,75}, btnSize, { 0,0,86,35 }, { 0.5f,0.5f });
	exit_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	exit_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	exit_btn->onMouseClick.Subscribe([this]() {Engine::Instance().m_input->ForceQuit(); });
	exit_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	exit_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	exit_btn->onMouseEnter.Subscribe([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	exit_btn->onMouseExit.Subscribe([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });
	exit_btn->SetLocalScale(btnScale);


	///// AddElements

	AddElementToCanvas(startNewGame_btn);
	AddElementToCanvas(startNewGame_text);
	AddElementToCanvas(continueGame_btn);
	AddElementToCanvas(continueGame_text);
	AddElementToCanvas(settings_btn);
	AddElementToCanvas(settings_text);
	AddElementToCanvas(exit_btn);
	AddElementToCanvas(exit_text);
}
