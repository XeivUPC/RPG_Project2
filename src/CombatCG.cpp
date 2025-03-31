#pragma once
#include "CombatCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleTime.h"
#include "TextureAtlas.h"
#include "DialogueSystem.h"
#include "DrawingTools.h"

#include "UIImage.h"
#include "UIButton.h"
#include "UITextBox.h"
#include "UISlider.h"

#include "Globals.h"
#include "Attack.h"

CombatCG::CombatCG(CombatSystem* _combatSystem)
{
	combat = _combatSystem;

	availableAttacks = vector<AttackSelectionButtonData>(4);
	availableTargets = vector<OverworldCharacter>(8);

	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("alagard");

	SDL_Texture* tex = Engine::Instance().m_assetsDB->GetTexture("dialogue_box");
	SDL_Texture* tex1 = Engine::Instance().m_assetsDB->GetTexture("btn_tex1");
	SDL_Texture* tex2 = Engine::Instance().m_assetsDB->GetTexture("slider_tex1");
	SDL_Texture* tex3 = Engine::Instance().m_assetsDB->GetTexture("toggle_tex1");

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*tex);

	combatBackground = new UIImage({ 0,0 }, { LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT },{0,0},false,{0,0,0,0},{200,200,200,255});
	AddElementToCanvas(combatBackground);

	attackSelectionBackground = new UIImage(*tex, {0,0}, textureSize);
	AddElementToCanvas(attackSelectionBackground);

	for (int i = 0; i < availableAttacks.size(); i++)
	{
		availableAttacks[i].btn = new UIButton(*tex1, {86 * (i / 2), 35 * (i % 2) }, {86,35}, {0,0,86,35});
		availableAttacks[i].btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });
		availableAttacks[i].btn->onMouseEnter.Subscribe([this, i]() {EnableAttackDescription(i); });
		availableAttacks[i].btn->onMouseExit.Subscribe([this]() {DisableAttackDescription(); });
		availableAttacks[i].btn->SetParent(attackSelectionBackground);
		availableAttacks[i].btn_text = new UITextBox(/*availableAttacks[i].attack->name*/"Default", *font, 12, {255,255,255,255}, {43,17}, {86,35}, {0.5f,0.5f}, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		availableAttacks[i].btn_text->SetParent(availableAttacks[i].btn);
	}

	description = new UITextBox("", *font, 10, { 255,255,255,255 }, { 172,0 }, { LOGIC_SCREEN_WIDTH - 172, 70 });
	description->SetParent(attackSelectionBackground);

	for (int i = 0; i < availableTargets.size(); i++)
	{
		Vector2Int position = { 0,0 };
		position.x = LOGIC_SCREEN_WIDTH / 4 + LOGIC_SCREEN_WIDTH / 2 * (i / 4);
		if (i % 2 == 0)
		{
			int i2 = i;
			if(i2 > 3)i2 -= 4;
			position.x += 48 - 96 * (i2 / 2);
		}
		else
		{
			int i2 = i;
			if (i2 > 3)i2 -= 4;
			position.y += 48 - 96 * (i2 / 3);
		}
		Vector2Int offset = { 0, 150 };
		availableTargets[i].btn = new UIButton(position+offset, {32, 62}, {0,0,0,0},{0.5f,0.5f},{255,255,255,0});
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
}

void CombatCG::UpdateCanvas()
{
	combat->UpdateCombat();
	
	UICanvas::UpdateCanvas();
}

void CombatCG::EnableAttackDescription(int attackIndex)
{
	//description->SetText(availableAttacks[attackIndex].attack->description);
	description->SetText("Default " + to_string(attackIndex));
}

void CombatCG::DisableAttackDescription()
{
	description->SetText("");
}
