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
	combat->onCombatStateChanged.Subscribe([this]() {OnStateChanged();});

	attackButtons = vector<AttackSelectionButtonData>(4);


	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("alagard");

	SDL_Texture* tex = Engine::Instance().m_assetsDB->GetTexture("uiBox3");
	SDL_Texture* tex1 = Engine::Instance().m_assetsDB->GetTexture("btn_tex1");
	SDL_Texture* tex2 = Engine::Instance().m_assetsDB->GetTexture("slider_tex1");
	SDL_Texture* tex3 = Engine::Instance().m_assetsDB->GetTexture("arrow_tex1");
	SDL_Texture* tex4 = Engine::Instance().m_assetsDB->GetTexture("toggle_tex1");


	combatBackground = new UIImage({ 0,0 }, { LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT },{0,0},false,{0,0,0,0},{200,200,200,255});
	AddElementToCanvas(combatBackground);

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*tex);
	attackSelectionBackground = new UIImage(*tex, {0,LOGIC_SCREEN_HEIGHT  - 88}, textureSize);
	AddElementToCanvas(attackSelectionBackground);

	for (int i = 0; i < attackButtons.size(); i++)
	{
		attackButtons[i].btn = new UIButton(*tex1, {8 + 86 * (i / 2) + 1 * (i / 2), 9 + 35 * (i % 2) +1 * (i % 2) }, {86,35}, {0,0,86,35});
		attackButtons[i].btn->AddRect(UIButton::ButtonStates::HOVER, { 86,0,86,35 });

		attackButtons[i].btn->onMouseEnter.Subscribe([this, i]() {EnableAttackDescription(i); });
		attackButtons[i].btn->onMouseExit.Subscribe([this,i]() {DisableAttackDescription(i); });
		attackButtons[i].btn->onMouseClick.Subscribe([this,i]() {SelectAttack(i); });

		attackButtons[i].btn->SetParent(attackSelectionBackground);
		attackButtons[i].btn_text = new UITextBox(/*availableAttacks[i].attack->name*/"Default", *font, 16, {255,255,255,255}, {43,17}, {86,35}, {0.5f,0.5f}, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		attackButtons[i].btn_text->SetParent(attackButtons[i].btn);
	}

	attackDescription = new UITextBox("", *font, 16, { 255,255,255,255 }, { 196,8 }, { 436, 72 });
	attackDescription->SetParent(attackSelectionBackground);


	SetUpCanvas();
	

	//attackConfirm = new UIButton(*tex3, {LOGIC_SCREEN_WIDTH/2,0}, { 19, 19 }, { 0,0,19,19 }, { 0.5f,0.5f });
	//attackConfirm->AddRect(UIButton::ButtonStates::HOVER, { 19,0,19,19 });
	//attackConfirm->SetParent(attackSelectionBackground);
	//attackConfirm->onMouseClick.Subscribe([this]() {SwitchCharacter(); });

	selectedCharacterIndicator = new UIImage(*tex3,{0,-60},{19,19},{0.5f,0.5f}, true, {0,0,19,19});
	selectedCharacterIndicator->localVisible = false;

	selectedCharacterIndicator->SetParent(combatBackground);



	endTurnButton = new UIButton(*tex4, { LOGIC_SCREEN_WIDTH - 20,LOGIC_SCREEN_HEIGHT - 20 }, { 19, 19 }, { 19,0,19,19 }, { 0.5f,0.5f });
	endTurnButton->localVisible = false;
	endTurnButton->onMouseClick.Subscribe([this]() {EndTurn(); });


	returnToSelectCharacterButton = new UIButton(*tex4, { LOGIC_SCREEN_WIDTH - 20,LOGIC_SCREEN_HEIGHT - 60 }, { 19, 19 }, { 19,0,19,19 }, { 0.5f,0.5f });
	returnToSelectCharacterButton->localVisible = false;
	returnToSelectCharacterButton->interactable = false;
	returnToSelectCharacterButton->onMouseClick.Subscribe([this]() {	SetSelectingTargetMode(false); });

	AddElementToCanvas(endTurnButton);
	AddElementToCanvas(returnToSelectCharacterButton);
}

void CombatCG::SetUpCanvas()
{
	for (int i = 0; i < charactersInCombat.size(); i++)
	{
		RemoveElementFromCanvas(charactersInCombat[i].btn);
		delete charactersInCombat[i].btn;
	}
	charactersInCombat.clear();


	for (const auto& characterTeam : combat->GetCharactersInCombat()) {
		int index = 0;
		for (auto& character : characterTeam.second)
		{
			charactersInCombat.emplace_back(OverworldCharacter((CombatSystem::CharacterReference*)&character, index));
			index++;
		}
	}

	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("alagard");
	SDL_Texture* tex2 = Engine::Instance().m_assetsDB->GetTexture("slider_tex1");

	for (int i = 0; i < charactersInCombat.size(); i++)
	{
		OverworldCharacter& overworldCharacter = charactersInCombat[i];
		int teamMember = overworldCharacter.position;

		Vector2 position = { 0,LOGIC_SCREEN_HEIGHT / 2 - 20 };
		Vector2 offset = { 0,0 };
		Vector2 displacement = { 60,55 };
		if (overworldCharacter.CharacterId->team == CombatSystem::Ally)
			position.x = LOGIC_SCREEN_WIDTH / 4;
		else
			position.x = LOGIC_SCREEN_WIDTH / 4 * 3;
		if (TeamMembersQuantity(overworldCharacter.CharacterId->team) > 1)
		{
			offset.x = std::cos((360 / TeamMembersQuantity(overworldCharacter.CharacterId->team)) * teamMember * M_PI / 180);
			if (overworldCharacter.CharacterId->team == CombatSystem::Enemy)
				offset.x *= -1;
			offset.y = std::sin((360 / TeamMembersQuantity(overworldCharacter.CharacterId->team)) * teamMember * M_PI / 180);
		}
		overworldCharacter.btn = new UIButton(position + Vector2{ offset.x * displacement.x,offset.y * displacement.y }, { 32, 62 }, { 0,0,0,0 }, { 0.5f,0.5f }, { 255,255,255,0 });
		overworldCharacter.btn->debug = true;
		overworldCharacter.btn->onMouseClick.Subscribe([this, i]() {SelectCharacter(i); });

		overworldCharacter.name = new UITextBox("Name", *font, 16, { 255,255,255,255 }, { 0,-38 }, { 32,15 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle, false);
		overworldCharacter.name->SetParent(overworldCharacter.btn);

		overworldCharacter.health = new UISlider(*tex2, { 0,38 }, { 40,4 }, { 3,4,40,4 }, *tex2, { 0,0 }, { 0,0,0,0 }, { 0,0,0,0 }, { 0.5f,0.5f }, 0.5f);
		overworldCharacter.health->SetParent(overworldCharacter.btn);


		AddElementToCanvas(overworldCharacter.btn);
	}
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
	UICanvas::UpdateCanvas();
}

void CombatCG::EnableAttackDescription(int attackIndex)
{
	//description->SetText(availableAttacks[attackIndex].attack->description);
	attackDescription->SetText("Default " + to_string(attackIndex));
	currentAttackDescription = attackIndex;
}

void CombatCG::DisableAttackDescription(int attackIndex)
{
	if (currentAttackDescription != attackIndex)
		return;
	attackDescription->SetText("");
}

int CombatCG::TeamMembersQuantity(CombatSystem::CharacterType _type)
{
	int quantity = 0;
	for (size_t i = 0; i < charactersInCombat.size(); i++)
	{
		if (charactersInCombat[i].CharacterId->team == _type)
			quantity++;
	}
	return quantity;
}

void CombatCG::SelectCharacter(int characterIndex)
{
	if (selectingTarget) {
		SelectTarget(charactersInCombat[characterIndex]);
		return;
	}
	if(charactersInCombat[characterIndex].CharacterId->team== CombatSystem::Enemy)
		return;
	if (selectedCharacter == &charactersInCombat[characterIndex])
		return;

	if (charactersInCombat[characterIndex].attackDone) {
		charactersInCombat[characterIndex].attackDone = false;
		if (attacksToExecute.count(charactersInCombat[characterIndex].CharacterId)) {
			attacksToExecute.erase(charactersInCombat[characterIndex].CharacterId);
		}

	}

	selectedCharacter = &charactersInCombat[characterIndex];
	selectedAttack = nullptr;
	printf("CharacterSelected\n");

	currentAttackDescription = -1;

	if(!selectedCharacterIndicator->localVisible)
		selectedCharacterIndicator->localVisible = true;
	selectedCharacterIndicator->SetParent(selectedCharacter->btn);


	////Change Attacks

}

void CombatCG::SelectAttack(int attackIndex)
{
	if (selectedCharacter == nullptr)
		return;
	if (selectedAttack == &attackButtons[attackIndex])
		return;
	selectedAttack = &attackButtons[attackIndex];

	
	printf("AttackSelected\n");

	//// Select Target of the attack
	seleccionableTargetsType = CombatSystem::Enemy;
	if (seleccionableTargetsType == CombatSystem::None) {
		selectedTargets.emplace_back(selectedCharacter);
		AddAttack();
		return;
	}

	/// Select Amount of tagets from attack
	selectionableTargets = 3;
	selectedTargets.clear();

	SetSelectingTargetMode(true);
}

void CombatCG::AddAttack()
{

	vector<CombatSystem::CharacterReference*> referenceTargets;

	for (size_t i = 0; i < selectedTargets.size(); i++)
	{
		referenceTargets.emplace_back(selectedTargets[i]->CharacterId);
	}
	attacksToExecute[selectedCharacter->CharacterId] = { selectedAttack->attack,referenceTargets };

	selectedCharacter->attackDone = true;

	SetSelectingTargetMode(false);

	selectedCharacterIndicator->localVisible = false;

	selectedCharacter = nullptr;
	selectedAttack = nullptr;

	printf("AttackAdded\n");
}

void CombatCG::SetSelectingTargetMode(bool mode)
{
	returnToSelectCharacterButton->interactable = mode;
	returnToSelectCharacterButton->localVisible = mode;
	selectingTarget = mode;

	//// Hide or disable buttons attack
}

void CombatCG::SelectTarget(OverworldCharacter& character)
{
	bool contains = std::find(selectedTargets.begin(), selectedTargets.end(), &character) != selectedTargets.end();
	if (contains) {
		DeselectTarget(character);
		return;
	}

	if (selectedTargets.size() == selectionableTargets)
		return;

	if (seleccionableTargetsType != CombatSystem::Both) {

		if (character.CharacterId->team != seleccionableTargetsType)
			return;
	}
	selectedTargets.push_back(&character);
	printf("Selected Targets amount -> %d\n", selectedTargets.size());
}

void CombatCG::DeselectTarget(OverworldCharacter& character)
{
	auto it = std::find(selectedTargets.begin(), selectedTargets.end(), &character);
	if (it != selectedTargets.end())
		selectedTargets.erase(it);
	printf("(UnSelected) Selected Targets amount -> %d\n", selectedTargets.size());

}



void CombatCG::EndTurn()
{
	for (auto& attackData : attacksToExecute)
	{
		combat->AddAttack(attackData.second.first, *attackData.first, attackData.second.second);
	}
	combat->ChangeState(CombatSystem::CombatState::ENEMY_TURN);
}

void CombatCG::OnStateChanged()
{
	switch (combat->GetCombatState())
	{
		case CombatSystem::START:
			
			break;
		case CombatSystem::PLAYER_TURN:
			SetSelectingTargetMode(false);
			currentAttackDescription = -1;
			attackDescription->SetText("");
			attackDescription->localVisible = true;
			attackDescription->interactable = true;

			for (size_t i = 0; i < attackButtons.size(); i++)
			{
				attackButtons[i].btn->localVisible = true;
				attackButtons[i].btn->interactable = true;
			}

			for (size_t i = 0; i < charactersInCombat.size(); i++)
			{
				charactersInCombat[i].btn->interactable = true;
				charactersInCombat[i].attackDone = false;
			}

			//attackButtonsCover->visible = false;
			//attackButtonsCover->interactable = false;

			endTurnButton->localVisible = true;
			endTurnButton->interactable = true;

			selectedCharacter = nullptr;
			selectedAttack = nullptr;

			break;
		case CombatSystem::ENEMY_TURN:
			SetSelectingTargetMode(false);
			attackDescription->SetText("");
			attackDescription->localVisible = false;
			attackDescription->interactable = false;

			for (size_t i = 0; i < attackButtons.size(); i++)
			{
				attackButtons[i].btn->localVisible = true;
				attackButtons[i].btn->interactable = false;
			}

			for (size_t i = 0; i < charactersInCombat.size(); i++)
			{
				charactersInCombat[i].btn->interactable = false;
				charactersInCombat[i].attackDone = false;
			}

			endTurnButton->localVisible = false;
			endTurnButton->interactable = false;

			//attackButtonsCover->visible = true;
			//attackButtonsCover->interactable = true;

			selectedCharacterIndicator->localVisible = false;

			selectedCharacter = nullptr;
			selectedAttack = nullptr;

			break;
		case CombatSystem::ATTACKS:
			break;
		case CombatSystem::EFFECTS:
			break;
		case CombatSystem::END_CHECK:
			break;
		case CombatSystem::VICTORY:
			break;
		case CombatSystem::DEFEAT:
			break;
	}
}
