#include "InventoryCG.h"
#include "Inventory.h"
#include "Party.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "TextureAtlas.h"

#include "UIImage.h"
#include "UITextBox.h"
#include "UISlider.h"
#include "UIToggle.h"
#include "UIButton.h"

#include "GameScene.h"
#include "PlayerCharacter.h"

InventoryCG::InventoryCG()
{
	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	Mix_Chunk* btn_enter = Engine::Instance().m_assetsDB->GetAudio("btn_enter");
	Mix_Chunk* btn_click = Engine::Instance().m_assetsDB->GetAudio("btn_click");

	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("uiBox6");
	SDL_Texture* close_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex2");


	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*bg_texture);
	container_image = new UIImage(*bg_texture, { 0,0 }, textureSize, { 0,0 });

	SetPosition(Vector2{ LOGIC_SCREEN_WIDTH / 2 - (float)textureSize.x / 2,LOGIC_SCREEN_HEIGHT / 2 - (float)textureSize.y / 2 });


	UITextBox* boxTitle_text = new UITextBox("Inventory", *textFont, 32, { 184,132,78,255 }, { textureSize.x / 2 , 0 }, { textureSize.x * 2, 50 }, { 0.5f,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	boxTitle_text->SetLocalScale(0.75f);
	boxTitle_text->SetParent(container_image);


	UIButton* close_btn = new UIButton(*close_texture, { 8, 8 }, { 15,15 }, { 0,0,15,15 }, { 0,0 });
	close_btn->AddRect(UIButton::ButtonStates::HOVER, { 15,0,15,15 });
	close_btn->AddRect(UIButton::ButtonStates::PRESSED, { 30,0,15,15 });
	close_btn->onMouseClick.Subscribe([this]() {isVisible = false; });
	close_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	close_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	close_btn->SetParent(container_image);



	AddElementToCanvas(container_image);
}

InventoryCG::~InventoryCG()
{
}

void InventoryCG::ChangeInventoryToTrack(Inventory*inventoryToTrack)
{
	inventory = inventoryToTrack;

	// Reload
}

void InventoryCG::ChangePartyToTrack(Party* partyToTrack)
{
	party = partyToTrack;
	UpdateCharacterSelectorSlots();
	// Reload
}

void InventoryCG::Reset()
{
}

void InventoryCG::CreateCharacterSelectorSlots()
{


	SDL_Texture* overlay = Engine::Instance().m_assetsDB->GetTexture("btn_tex6");

	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");


	Vector2 anchor = { 19 ,87 };
	Vector2 slotSize = Engine::Instance().m_assetsDB->GetTextureSize(*overlay);
	Vector2 spacing = { 0, 5 };

	const vector<CharacterDatabase::CharacterData*> memebers = party->GetMemebers();


	for (int i = 0; i < memebers.size(); ++i)
	{

		if (selectorSlots.size() > i)
			continue;

		CharacterDatabase::CharacterData* charData = memebers[i];

		Vector2 offset = { 0,(slotSize.y + spacing.y) * i };
		Vector2 position = { anchor.x + offset.x, anchor.y + offset.y };

		UICharacterSelectorSlot slot;
		   
		slot.characterId = charData->id;
		slot.characterSelect = new UIButton(position, slotSize, { 0,0,0,0 }, { 0,0 }, { 0,0,0,0 });
		

		slot.characterOverlay = new UIImage(*overlay, { 0, 0 }, slotSize);
		slot.characterOverlay->SetParent(slot.characterSelect);

		slot.characterName = new UITextBox(charData->name, *textFont, 16, { 184,132,78,255 }, { 0,5 }, { (int)slotSize.x,32}, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		slot.characterName->SetParent(slot.characterSelect);

		slot.characterSelect->SetParent(container_image);

		selectorSlots.emplace_back(slot);
	}
}

void InventoryCG::UpdateCharacterSelectorSlots()
{
	CreateCharacterSelectorSlots();

	for (int i = 0; i < selectorSlots.size(); ++i) {
		selectorSlots[i].characterName->localVisible = false;
		selectorSlots[i].characterOverlay->localVisible = false;

		selectorSlots[i].characterSelect->isEnabled = false;
		selectorSlots[i].characterSelect->localVisible = false;

	}

	const vector<CharacterDatabase::CharacterData*> memebers = party->GetMemebers();
	for (int i = 0; i < memebers.size(); ++i)
	{
		CharacterDatabase::CharacterData* charData = memebers[i];
		UICharacterSelectorSlot& slot = selectorSlots[i];

		slot.characterId = charData->id;
		int id = slot.characterId;

		slot.characterName->SetText(charData->name);


		selectorSlots[i].characterName->localVisible = true;
		selectorSlots[i].characterOverlay->localVisible = true;

		selectorSlots[i].characterSelect->isEnabled = true;
		selectorSlots[i].characterSelect->localVisible = true;

	}
}
