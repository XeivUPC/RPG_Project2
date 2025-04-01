
#include "CombatCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleTime.h"
#include "ModuleInput.h"
#include "TextureAtlas.h"
#include "DrawingTools.h"

#include "UIImage.h"
#include "UIButton.h"
#include "UITextBox.h"
#include "UISlider.h"
#include "UIToggle.h"

#include "Globals.h"
#include "AttackList.h"
#include "CharacterDatabase.h"

#include <cmath>

CombatCG::CombatCG(CombatSystem* _combatSystem)
{
	combat = _combatSystem;
}

void CombatCG::UpdateCanvas()
{
	UICanvas::UpdateCanvas();
}

void CombatCG::LoadCanvas()
{
	//// SetBackground
	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("");
	//combatBg = new UIImage(*bg_texture, {0,0}, {LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT});
	combatBg = new UIImage({ 0,0 }, { LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT }, { 0,0 }, false, { 0,0,0,0 }, { 200,200,200,255 });
	AddElementToCanvas(combatBg);

	////CreateCombatLayout
	SDL_Texture* layout_texture = Engine::Instance().m_assetsDB->GetTexture("uiBox3");
	combatLayout = new UIImage(*layout_texture, { 0, LOGIC_SCREEN_HEIGHT - 89 }, {640,89});
	combatLayout->SetParent(combatBg);

	//// Create AttackButtons
	for (int i = 0; i < 4; i++)
	{
		Vector2Int btn_position = { 8 + 86 * (i / 2) + 1 * (i / 2), 9 + 35 * (i % 2) + 1 * (i % 2) };
		Attack* attackValue = nullptr;
		attackButtons.emplace_back(CreateUIAttackButton(attackValue, btn_position));

		attackButtons[i].btn->SetParent(combatLayout);
	}

	/// CreateAttackInformation
	CreateUIAttackInformation();
	HideAttackInformation(nullptr);
	attackInfo.name->SetParent(combatLayout);

	////CreateCharactersUI

	for (int i = 0; i < charactersSlot.size(); i++)
	{
		RemoveElementFromCanvas(charactersSlot[i].character);
		delete charactersSlot[i].character;
	}
	charactersSlot.clear();

	for (const auto& characterTeam : combat->GetCharactersInCombat()) {
		int index = 0;
		for (auto& character : characterTeam.second)
		{
			Vector2Int position=  { 100,100 };
			UICharacterSlot& reference = charactersSlot.emplace_back(CreateUICharacterSlot((CombatSystem::CharacterReference*)&character, position));
			//reference.character->onMouseClick.Subscribe([this]() {});
			index++;
		}
	}


	//// Add Elements
	
}

CombatCG::UIAttackButton CombatCG::CreateUIAttackButton(Attack* value, Vector2 btn_position)
{
	SDL_Texture* btn_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex1");
	TTF_Font* btn_font = Engine::Instance().m_assetsDB->GetFont("alagard");

	Vector2Int btn_size = { 86,35 };

	int font_size = 16;
	SDL_Color font_color = { 255,255,255,255 };
	UITextBox::HorizontalAlignment hAligment = UITextBox::HorizontalAlignment::Middle;
	UITextBox::VerticalAlignment vAligment = UITextBox::VerticalAlignment::Middle;

	UIButton* button = new UIButton(*btn_texture, btn_position, btn_size, {0,0,btn_size.x,btn_size.y});
	button->AddRect(UIButton::ButtonStates::HOVER, { btn_size.x,0,btn_size.x,btn_size.y });
	UITextBox* attackName = new UITextBox(value ? value->name : "", *btn_font, font_size, font_color, btn_size / 2, btn_size, {0.5f,0.5f}, hAligment, vAligment);
	
	button->onMouseEnter.Subscribe([this, value]() {ShowAttackInformation(value);});
	button->onMouseExit.Subscribe([this, value]() {HideAttackInformation(value);});
	//button->onMouseClick.Subscribe();

	attackName->SetParent(button);
	if (!value)
		button->isEnabled = false;
	UIAttackButton attackButtonGroup = { button, attackName, value };

	return move(attackButtonGroup);
}

void CombatCG::CreateUIAttackInformation()
{
	TTF_Font* btn_font = Engine::Instance().m_assetsDB->GetFont("alagard");

	int font_size = 16;
	SDL_Color font_color = { 255,255,255,255 };
	UITextBox::HorizontalAlignment hAligment = UITextBox::HorizontalAlignment::Left;
	UITextBox::VerticalAlignment vAligment = UITextBox::VerticalAlignment::Top;

	UITextBox* attackName = new UITextBox("Name", *btn_font, font_size, font_color, {199,10}, { 143,16 }, { 0,0 }, hAligment, vAligment);
	UITextBox* attackDescription = new UITextBox("Description", *btn_font, font_size, font_color, {0,24}, { 431,46 }, { 0,0 }, hAligment, vAligment);
	
	//hAligment = UITextBox::HorizontalAlignment::Right;
	UITextBox* attackPower = new UITextBox("Pow: 30", *btn_font, font_size, font_color, {299,0}, { 72,16 }, { 0,0 }, hAligment, vAligment,false);
	UITextBox* attackAccuracy = new UITextBox("Acc: 100%", *btn_font, font_size, font_color, { 361,0 }, { 72,16 }, {0,0}, hAligment, vAligment,false);

	attackDescription->SetParent(attackName);
	attackPower->SetParent(attackName);
	attackAccuracy->SetParent(attackName);

	attackInfo = { attackName,attackDescription,attackPower,attackAccuracy };
}

CombatCG::UICharacterSlot CombatCG::CreateUICharacterSlot(CombatSystem::CharacterReference* value, Vector2 slot_position)
{
	SDL_Texture* overlay_texture = Engine::Instance().m_assetsDB->GetTexture("combat_characterUI");
	SDL_Texture* effects_texture = Engine::Instance().m_assetsDB->GetTexture("effects_toggle");
	SDL_Texture* charSelect_texture = Engine::Instance().m_assetsDB->GetTexture("arrow_tex1");

	TTF_Font* btn_font = Engine::Instance().m_assetsDB->GetFont("alagard");

	int font_size = 16;
	SDL_Color font_color = { 255,255,255,255 };
	UITextBox::HorizontalAlignment hAligment = UITextBox::HorizontalAlignment::Middle;
	UITextBox::VerticalAlignment vAligment = UITextBox::VerticalAlignment::Middle;

	UIImage* overlay = new UIImage(*overlay_texture, slot_position, { 69,11 }, { 0,0 }, true, { 0,0,69,11 });

	UITextBox* slotLvl = new UITextBox("10", *btn_font, font_size, font_color, { 2,2 }, { 18,18 }, { 0,0 }, hAligment, vAligment);
	slotLvl->SetLocalScale(0.5f);

	hAligment = UITextBox::HorizontalAlignment::Left;
	vAligment = UITextBox::VerticalAlignment::Top;
	UITextBox* slotName = new UITextBox("Cassian", *btn_font, font_size, font_color, { 0,0 }, { 69,18 }, { 0,1 }, hAligment, vAligment);
	slotName->SetLocalScale(0.5f);
	
	UIToggle* poisonToggle = new UIToggle(*effects_texture, { 53,4 }, { 5,3 }, { 0,0,5,3 }, { 5,0,5,3 }, {0,0},false);
	poisonToggle->interactable = false;
	UIToggle* burnToggle = new UIToggle(*effects_texture, {58,4}, { 5,3 },{ 0,3,5,3 }, { 5,3,5,3 }, { 0,0 }, false);
	burnToggle->interactable = false;
	UIToggle* regenerationToggle = new UIToggle(*effects_texture, {63,4}, {5,3}, { 0,6,5,3 }, { 5,6,5,3 }, { 0,0 }, false);
	regenerationToggle->interactable = false;


	UIImage* hpBar = new UIImage({ 11,4 }, { 41,3 }, { 0,0 }, false, { 0,0,0,0 }, {255,0,0,255});
	UIImage* selectedCharacterIndicator = new UIImage(*charSelect_texture, { -1,5 }, {19,19}, { 1,0.5f });
	selectedCharacterIndicator->localVisible = false;
	
	UIImage* selectedCharacterTarget = new UIImage(*overlay_texture, {0,0}, {69,11}, {0,0}, true, {0,11,69,11});
	selectedCharacterTarget->localVisible = false;

	UIButton* characterBtn = new UIButton({35,-7}, { 32, 62 }, {0,0,0,0}, { 0.5f,1 }, {0,0,0,100});

	selectedCharacterTarget->SetParent(overlay);
	characterBtn->SetParent(overlay);
	slotLvl->SetParent(overlay);
	slotName->SetParent(overlay);
	hpBar->SetParent(overlay);

	poisonToggle->SetParent(overlay);
	burnToggle->SetParent(overlay);
	regenerationToggle->SetParent(overlay);

	selectedCharacterIndicator->SetParent(overlay);


	AddElementToCanvas(overlay);

	return UICharacterSlot();
}

void CombatCG::ShowAttackInformation(Attack* attackSelected)
{
	attackInfo.name->localVisible = true;

	attackInfo.currentAttack = attackSelected;

	attackInfo.name->SetText(attackSelected->name);
	attackInfo.description->SetText(attackSelected->description);
	attackInfo.power->SetText("Pow: "+to_string(attackSelected->damage));
	attackInfo.accuracy->SetText("Acc: " + to_string(attackSelected->accuracity) + "%");
}

void CombatCG::HideAttackInformation(Attack* attackDeselected)
{
	if (attackInfo.currentAttack != attackDeselected)
		return;
	attackInfo.name->localVisible = false;
	attackInfo.currentAttack = nullptr;
}


