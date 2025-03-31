#pragma once
#include "CombatCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleTime.h"
#include "ModuleInput.h"
#include "TextureAtlas.h"
#include "DialogueSystem.h"
#include "DrawingTools.h"

#include "UIImage.h"
#include "UIButton.h"
#include "UITextBox.h"
#include "UISlider.h"

#include "Globals.h"
#include "Attack.h"

#include <cmath>

CombatCG::CombatCG(CombatSystem* _combatSystem)
{
	combat = _combatSystem;

	availableAttacks = vector<AttackSelectionButtonData>(4);
	availableTargets = vector<OverworldCharacter>{
		OverworldCharacter(0, CombatSystem::CharacterType::Ally),
		OverworldCharacter(1, CombatSystem::CharacterType::Ally),
		OverworldCharacter(2, CombatSystem::CharacterType::Ally),
		OverworldCharacter(0, CombatSystem::CharacterType::Enemy),
		OverworldCharacter(1, CombatSystem::CharacterType::Enemy)
	};

	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("alagard");

	SDL_Texture* tex = Engine::Instance().m_assetsDB->GetTexture("uiBox3");
	SDL_Texture* tex1 = Engine::Instance().m_assetsDB->GetTexture("btn_tex1");
	SDL_Texture* tex2 = Engine::Instance().m_assetsDB->GetTexture("slider_tex1");
	SDL_Texture* tex3 = Engine::Instance().m_assetsDB->GetTexture("toggle_tex1");


	combatBackground = new UIImage({ 0,0 }, { LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT },{0,0},false,{0,0,0,0},{200,200,200,255});
	AddElementToCanvas(combatBackground);

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*tex);
	attackSelectionBackground = new UIImage(*tex, {0,LOGIC_SCREEN_HEIGHT  - 88}, textureSize);
	AddElementToCanvas(attackSelectionBackground);

	for (int i = 0; i < availableAttacks.size(); i++)
	{
		availableAttacks[i].btn = new UIButton(*tex1, {8 + 86 * (i / 2) + 1 * (i / 2), 9 + 35 * (i % 2) +1 * (i % 2) }, {86,35}, {0,0,86,35});
		availableAttacks[i].btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
		availableAttacks[i].btn->onMouseEnter.Subscribe([this, i]() {EnableAttackDescription(i); });
		availableAttacks[i].btn->onMouseExit.Subscribe([this,i]() {DisableAttackDescription(i); });
		availableAttacks[i].btn->SetParent(attackSelectionBackground);
		availableAttacks[i].btn_text = new UITextBox(/*availableAttacks[i].attack->name*/"Default", *font, 12, {255,255,255,255}, {43,17}, {86,35}, {0.5f,0.5f}, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		availableAttacks[i].btn_text->SetParent(availableAttacks[i].btn);
	}

	description = new UITextBox("", *font, 10, { 255,255,255,255 }, { 196,8 }, { 436, 72 });
	description->SetParent(attackSelectionBackground);

	for (int i = 0; i < availableTargets.size(); i++)
	{
		int teamMember = i;
		if(teamMember >= TeamMembersQuantity(CombatSystem::Ally))
			teamMember -= TeamMembersQuantity(CombatSystem::Ally);
		Vector2 position = { 0,LOGIC_SCREEN_HEIGHT/2 };
		Vector2 offset = { 0,0 };
		Vector2 displacement = { 60,55 };
		if (availableTargets[i].CharacterId.team == CombatSystem::Ally)
			position.x = LOGIC_SCREEN_WIDTH / 4;
		else
			position.x = LOGIC_SCREEN_WIDTH / 4 * 3;

		offset.x = std::cos((360 / TeamMembersQuantity(availableTargets[i].CharacterId.team)) * teamMember * M_PI / 180);
		if (availableTargets[i].CharacterId.team == CombatSystem::Enemy)
			offset.x *= -1;
		offset.y = std::sin((360 / TeamMembersQuantity(availableTargets[i].CharacterId.team)) * teamMember * M_PI / 180);

		printf("");
		availableTargets[i].btn = new UIButton(position + Vector2{offset.x* displacement.x,offset.y*displacement.y}, { 32, 62 }, { 0,0,0,0 }, { 0.5f,0.5f }, { 255,255,255,0 });
		availableTargets[i].btn->debug = true;
		AddElementToCanvas(availableTargets[i].btn);

		availableTargets[i].name = new UITextBox("Name", *font, 12, { 255,255,255,255 }, { 0,-38 }, { 32,15 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle, false);
		availableTargets[i].name->SetParent(availableTargets[i].btn);

		availableTargets[i].health = new UISlider(*tex2, { 0,38 }, { 40,4 }, { 3,4,40,4 }, *tex2, { 0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0.5f,0.5f }, 0.5f);
		availableTargets[i].health->SetParent(availableTargets[i].btn);
	}

	attackConfirm = new UIButton(*tex3, {LOGIC_SCREEN_WIDTH/2,0}, { 19, 19 }, { 0,0,19,19 }, { 0.5f,0.5f });
	attackConfirm->AddRect(UIButton::ButtonStates::HOVER, { 19,0,19,19 });
	attackConfirm->SetParent(attackSelectionBackground);
	attackConfirm->onMouseClick.Subscribe([this]() {SwitchCharacter(); });

	currentCharacter = new UIImage(*tex3,{0,-60},{19,19},{0.5f,0.5f}, true, {19,0,19,19});
	currentCharacter->SetParent(availableTargets[currentCombatCharacter].btn);
}

void CombatCG::UpdateCanvas()
{
	combat->UpdateCombat();
	switch (combat->GetCombatState())
	{
	case CombatSystem::START:
		currentCombatCharacter = 0;

		break;
	case CombatSystem::PLAYER_TURN:

		break;
	default:
		break;
	}
	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_A)==KEY_DOWN)
		SwitchCharacter();
	UICanvas::UpdateCanvas();
}

void CombatCG::EnableAttackDescription(int attackIndex)
{
	//description->SetText(availableAttacks[attackIndex].attack->description);
	description->SetText("Default " + to_string(attackIndex));
	currentAttackDescription = attackIndex;
}

void CombatCG::DisableAttackDescription(int attackIndex)
{
	if (currentAttackDescription != attackIndex)
		return;
	description->SetText("");
}

void CombatCG::SwitchCharacter()
{
	currentCombatCharacter++;
	if (currentCombatCharacter == 4)
		combat->ChangeState(CombatSystem::CombatState::ENEMY_TURN);
	currentCharacter->SetParent(availableTargets[currentCombatCharacter].btn);
}

int CombatCG::TeamMembersQuantity(CombatSystem::CharacterType _type)
{
	int quantity = 0;
	for (size_t i = 0; i < availableTargets.size(); i++)
	{
		if (availableTargets[i].CharacterId.team == _type)
			quantity++;
	}
	return quantity;
}
