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
	Vector2Int anchorPoint = { LOGIC_SCREEN_WIDTH / 2, LOGIC_SCREEN_HEIGHT / 2 };

	UITextBox* startNewGame_text = new UITextBox("New Game", *textFont, 32, { 184,132,78,255 }, anchorPoint + Vector2Int{ 0,-150 + 3 }, { 57 * 3,23 * 3 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UIButton* startNewGame_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{0,-150}, { 57,23 }, { 0,0,57,23 }, { 0.5f,0.5f });
	startNewGame_btn->AddRect(UIButton::ButtonStates::HOVER, { 57,0,57,23 });
	startNewGame_btn->AddRect(UIButton::ButtonStates::PRESSED, { 114,0,57,23 });
	startNewGame_btn->onMouseClick.emplace_back([this]() {Engine::Instance().s_title->StartGame(true); });
	startNewGame_btn->onMouseClick.emplace_back([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	startNewGame_btn->onMouseEnter.emplace_back([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	startNewGame_btn->onMouseEnter.emplace_back([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	startNewGame_btn->onMouseExit.emplace_back([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });
	startNewGame_btn->SetLocalScale(3);
	

	UITextBox* continueGame_text = new UITextBox("Continue", *textFont, 32, { 184,132,78,255 }, anchorPoint + Vector2Int{ 0,-50 + 3 }, { 57 * 3,23 * 3 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UIButton* continueGame_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,-50 }, { 57,23 }, { 0,0,57,23 }, { 0.5f,0.5f });
	continueGame_btn->AddRect(UIButton::ButtonStates::HOVER, { 57,0,57,23 });
	continueGame_btn->AddRect(UIButton::ButtonStates::PRESSED, { 114,0,57,23 });
	continueGame_btn->onMouseClick.emplace_back([this]() {Engine::Instance().s_title->StartGame(false); });
	continueGame_btn->onMouseClick.emplace_back([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	continueGame_btn->onMouseEnter.emplace_back([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	continueGame_btn->onMouseEnter.emplace_back([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	continueGame_btn->onMouseExit.emplace_back([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });
	continueGame_btn->SetLocalScale(3);


	UITextBox* settings_text = new UITextBox("Settings", *textFont, 32, { 184,132,78,255 }, anchorPoint + Vector2Int{ 0,50 + 3 }, { 57 * 3,23 * 3 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UIButton* settings_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,50 }, { 57,23 }, { 0,0,57,23 }, { 0.5f,0.5f });
	settings_btn->AddRect(UIButton::ButtonStates::HOVER, { 57,0,57,23 });
	settings_btn->AddRect(UIButton::ButtonStates::PRESSED, { 114,0,57,23 });
	settings_btn->onMouseClick.emplace_back([this]() {settingsReference->visible = true; });
	settings_btn->onMouseClick.emplace_back([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	settings_btn->onMouseEnter.emplace_back([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	settings_btn->onMouseEnter.emplace_back([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	settings_btn->onMouseExit.emplace_back([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });
	settings_btn->SetLocalScale(3);


	UITextBox* exit_text = new UITextBox("Exit", *textFont, 32, { 184,132,78,255 }, anchorPoint + Vector2Int{ 0,150 + 3 }, { 57 * 3,23 * 3 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	UIButton* exit_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,150}, { 57,23 }, { 0,0,57,23 }, { 0.5f,0.5f });
	exit_btn->AddRect(UIButton::ButtonStates::HOVER, { 57,0,57,23 });
	exit_btn->AddRect(UIButton::ButtonStates::PRESSED, { 114,0,57,23 });
	exit_btn->onMouseClick.emplace_back([this]() {Engine::Instance().m_input->ForceQuit(); });
	exit_btn->onMouseClick.emplace_back([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	exit_btn->onMouseEnter.emplace_back([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	exit_btn->onMouseEnter.emplace_back([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	exit_btn->onMouseExit.emplace_back([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });
	exit_btn->SetLocalScale(3);


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
