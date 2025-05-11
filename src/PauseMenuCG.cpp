#include "PauseMenuCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleCursor.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "DrawingTools.h"

#include "GameScene.h"
#include "PlayerCharacter.h"

#include "UIImage.h"
#include "UIButton.h"
#include "UITextBox.h"

#include "SettingsCG.h"
#include "PartyCG.h"
#include "InventoryCG.h"
#include "SaveLoadCG.h"

#include "Globals.h"

PauseMenuCG::PauseMenuCG(int _renderLayer)
{

	renderLayer = _renderLayer;
	//// Create SubCanvas
	settings = new SettingsCG();
	settings->renderLayer = renderLayer;
	submenus["Settings"] = settings;

	party = new PartyCG();
	submenus["Party"] = party;
	party->renderLayer = renderLayer;

	inventory = new InventoryCG();
	submenus["Inventory"] = inventory;
	inventory->renderLayer = renderLayer;

	saveLoad = new SaveLoadCG();
	submenus["SaveLoad"] = saveLoad;
	saveLoad->renderLayer = renderLayer;

	Engine::Instance().m_render->SetRenderQueueDirty();

	settings->SetPosition( Vector2{ 152,0});
	party->SetPosition( Vector2{ 152,0});
	inventory->SetPosition( Vector2{ 152,0});
	saveLoad->SetPosition( Vector2{ 152,0});

	///// AssetsLoading
	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	Mix_Chunk* btn_enter = Engine::Instance().m_assetsDB->GetAudio("btn_enter");
	Mix_Chunk* btn_click = Engine::Instance().m_assetsDB->GetAudio("btn_click");

	SDL_Texture* btn_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex1");
	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("uiBox2");



	///// Background
	UIImage* bg_image = new UIImage({ 152,0 }, { 488,360 }, { 0,0 }, true, { 0,0,488,360 }, { 0,0,0,199 });
	bg_image->SetLocalScale(1);

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*bg_texture);
	UIImage* bgButtons_image = new UIImage(*bg_texture,{ 0,0 }, textureSize, { 0,0 });
	bg_image->SetLocalScale(1);




	///// Title

	UITextBox* boxTitle_text = new UITextBox("Paused", *textFont, 32, { 184,132,78,255 }, { 75 , 23 }, { textureSize.x * 2, 50 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	boxTitle_text->SetLocalScale(0.75f);

	///// Buttons
	Vector2Int anchorPoint = {  33, 49 };
	float btnScale = 1;
	int btnOffset = 50;
	Vector2Int btnSize = { 86,35 };

	UIButton* continueGame_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,0 * btnOffset }, btnSize, { 0,0,86,35 }, { 0,0 });
	UITextBox* resumeGame_text = new UITextBox("Resume", *textFont, 16, { 184,132,78,255 }, {0,2}, btnSize * btnScale, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	resumeGame_text->SetParent(continueGame_btn);
	continueGame_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	continueGame_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	continueGame_btn->onMouseClick.Subscribe([this]() {Engine::Instance().s_game->SetPreviousState(); });
	continueGame_btn->onMouseClick.Subscribe([this]() {CloseAllSubmenus(); });
	continueGame_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	continueGame_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	continueGame_btn->onMouseEnter.Subscribe([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	continueGame_btn->onMouseExit.Subscribe([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });


	UIButton* teamGame_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,1 * btnOffset }, btnSize, { 0,0,86,35 }, { 0,0 });
	UITextBox* teamGame_text = new UITextBox("Party", *textFont, 16, { 184,132,78,255 }, { 0,2 }, btnSize * btnScale, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	teamGame_text->SetParent(teamGame_btn);
	teamGame_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	teamGame_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	teamGame_btn->onMouseClick.Subscribe([this]() {OpenSubmenu("Party"); party->Reset();  party->ChangePartyToTrack(Engine::Instance().s_game->GetPlayer()->party); });
	teamGame_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	teamGame_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	teamGame_btn->onMouseEnter.Subscribe([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	teamGame_btn->onMouseExit.Subscribe([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });



	UIButton* inventoryGame_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,2 * btnOffset }, btnSize, { 0,0,86,35 }, { 0,0 });
	UITextBox* inventoryGame_text = new UITextBox("Inventory", *textFont, 16, { 184,132,78,255 }, { 0,2 }, btnSize * btnScale, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	inventoryGame_text->SetParent(inventoryGame_btn);
	inventoryGame_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	inventoryGame_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	inventoryGame_btn->onMouseClick.Subscribe([this]() {OpenSubmenu("Inventory"); inventory->Reset();  inventory->ChangeInventoryToTrack(Engine::Instance().s_game->GetPlayer()->inventory); });
	inventoryGame_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	inventoryGame_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	inventoryGame_btn->onMouseEnter.Subscribe([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	inventoryGame_btn->onMouseExit.Subscribe([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });



	UIButton* saveLoadGame_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,3 * btnOffset }, btnSize, { 0,0,86,35 }, { 0,0 });
	UITextBox* saveLoadGame_text = new UITextBox("Save/Load", *textFont, 16, { 184,132,78,255 }, { 0,2 }, btnSize * btnScale, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	saveLoadGame_text->SetParent(saveLoadGame_btn);
	saveLoadGame_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	saveLoadGame_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	saveLoadGame_btn->onMouseClick.Subscribe([this]() {OpenSubmenu("SaveLoad");});
	saveLoadGame_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	saveLoadGame_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	saveLoadGame_btn->onMouseEnter.Subscribe([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	saveLoadGame_btn->onMouseExit.Subscribe([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });


	UIButton* settingsGame_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,4 * btnOffset }, btnSize, { 0,0,86,35 }, { 0,0 });
	UITextBox* settingsGame_text = new UITextBox("Settings", *textFont, 16, { 184,132,78,255 }, { 0,2 }, btnSize * btnScale, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	settingsGame_text->SetParent(settingsGame_btn);
	settingsGame_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	settingsGame_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	settingsGame_btn->onMouseClick.Subscribe([this]() {OpenSubmenu("Settings"); Engine::Instance().m_audio->SetAudioBoost(1); });
	settingsGame_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	settingsGame_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	settingsGame_btn->onMouseEnter.Subscribe([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	settingsGame_btn->onMouseExit.Subscribe([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });


	UIButton* mainMenuGame_btn = new UIButton(*btn_texture, anchorPoint + Vector2Int{ 0,5* btnOffset }, btnSize, { 0,0,86,35 }, { 0,0 });
	UITextBox* mainMenuGame_text = new UITextBox("Main Menu", *textFont, 16, { 184,132,78,255 }, { 0,2 }, btnSize * btnScale, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	mainMenuGame_text->SetParent(mainMenuGame_btn);
	mainMenuGame_btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
	mainMenuGame_btn->AddRect(UIButton::ButtonStates::PRESSED, { 172,0,86,35 });
	mainMenuGame_btn->onMouseClick.Subscribe([this]() {Engine::Instance().s_game->ExitGame(); });
	mainMenuGame_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	mainMenuGame_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	mainMenuGame_btn->onMouseEnter.Subscribe([this]() {Engine::Instance().m_cursor->SelectCursor("hand_cursor"); });
	mainMenuGame_btn->onMouseExit.Subscribe([this]() {Engine::Instance().m_cursor->SelectDefaultCursor(); });

	///// AddElements
	AddElementToCanvas(bg_image);
	AddElementToCanvas(bgButtons_image);
	AddElementToCanvas(boxTitle_text);
	AddElementToCanvas(continueGame_btn);
	AddElementToCanvas(teamGame_btn);
	AddElementToCanvas(inventoryGame_btn);
	AddElementToCanvas(saveLoadGame_btn);
	AddElementToCanvas(settingsGame_btn);
	AddElementToCanvas(mainMenuGame_btn);
}

PauseMenuCG::~PauseMenuCG()
{
	delete settings;
	delete party;
	delete inventory;
	delete saveLoad;
}

void PauseMenuCG::Init()
{
	Engine::Instance().m_audio->SetAudioBoost(audio_boost);
}

void PauseMenuCG::CloseAllSubmenus()
{
	for (auto& submenu : submenus)
	{
		submenu.second->isVisible = false;
	}
}

void PauseMenuCG::OpenSubmenu(string menuName)
{
	CloseAllSubmenus();
	submenus[menuName]->isVisible = true;
	submenus[menuName]->renderLayer = renderLayer + 1;

	currentSubmenu = submenus[menuName];
}

void PauseMenuCG::UpdateCanvas()
{
	if (currentSubmenu != nullptr)
		currentSubmenu->UpdateCanvas();

	if(Engine::Instance().m_audio->GetAudioBoost() != audio_boost && !settings->isVisible)
		Engine::Instance().m_audio->SetAudioBoost(audio_boost);

	UICanvas::UpdateCanvas();
}
