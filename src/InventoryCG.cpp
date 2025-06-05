#include "InventoryCG.h"
#include "Inventory.h"
#include "Party.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "TextureAtlas.h"

#include "UIImage.h"
#include "UITextBox.h"
#include "UISlider.h"
#include "UIToggle.h"
#include "UIButton.h"

#include "GameScene.h"
#include "PlayerCharacter.h"

InventoryCG::InventoryCG(int _renderLayer)
{


	renderLayer = _renderLayer;
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

	CreateCharacterSlot();
	CreateExtras();


	AddElementToCanvas(container_image);
	AddElementToCanvas(itemHoldingImage);
}

InventoryCG::~InventoryCG()
{
}

void InventoryCG::UpdateCanvas()
{
	if (currentHoldedSlot != nullptr) {

		itemHoldingImage->SetLocalPosition(Engine::Instance().m_input->GetMousePosition() - GetPosition());
		
		if (Engine::Instance().m_input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
			if(mouseOverSlot==nullptr)
				OnItemSelected(nullptr);
			else
				OnItemSelected(mouseOverSlot);

		}
	}
	UICanvas::UpdateCanvas();
}

void InventoryCG::ChangeInventoryToTrack(Inventory*inventoryToTrack)
{
	playerInventory = inventoryToTrack;
	UpdateItemSlots();
	SendElementToFront(itemHoldingImage);
}

void InventoryCG::ChangePartyToTrack(Party* partyToTrack)
{
	party = partyToTrack;
	GoToMemebersOffset(0);
	UpdateCharacterSelectorSlots();
}

void InventoryCG::Reset()
{
	membersOffset = -1;

	GoToMemebersOffset(0);
	UpdateCharacterSelectorSlots();
	UpdateItemSlots();

	UpdateCharacterSlot("");

}

void InventoryCG::CreateCharacterSelectorSlots()
{


	SDL_Texture* overlay = Engine::Instance().m_assetsDB->GetTexture("btn_tex6");

	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");


	Vector2 anchor = { 19 ,87 };
	Vector2 slotSize = Engine::Instance().m_assetsDB->GetTextureSize(*overlay);
	slotSize.x /= 3;
	Vector2 spacing = { 0, 5 };

	const vector<CharacterDatabase::CharacterDefinition*> memebers = party->GetMemebers();


	for (int i = 0; i < memebers.size(); ++i)
	{

		if (selectorSlots.size() > i)
			continue;

		CharacterDatabase::CharacterDefinition* charData = memebers[i];

		Vector2 offset = { 0,(slotSize.y + spacing.y) * i };
		Vector2 position = { anchor.x + offset.x, anchor.y + offset.y };

		UICharacterSelectorSlot slot;
		   
		slot.characterId = charData->id;
		//slot.characterData = charData;
		slot.characterSelect = new UIButton(*overlay, position, slotSize, {0,0,(int)slotSize.x, (int)slotSize.y});
		slot.characterSelect->AddRect(UIButton::ButtonStates::HOVER, { (int)slotSize.x,0,(int)slotSize.x, (int)slotSize.y });
		slot.characterSelect->AddRect(UIButton::ButtonStates::PRESSED, { (int)slotSize.x*2,0,(int)slotSize.x, (int)slotSize.y });
		slot.characterSelect->onMouseClick.Subscribe([this, slot]() {UpdateCharacterSlot(slot.characterId); });

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

		selectorSlots[i].characterSelect->localVisible = false;
		selectorSlots[i].characterSelect->interactable = false;

	}

	const vector<CharacterDatabase::CharacterDefinition*> memebers = party->GetMemebers();

	int maxIndex = membersOffset + membersByPage;
	if (maxIndex > memebers.size()) {
		maxIndex = (int)memebers.size();
	}

	for (int i = membersOffset; i < maxIndex; ++i)
	{
		CharacterDatabase::CharacterDefinition* charData = memebers[i];
		int realIndex = i - membersOffset;
		UICharacterSelectorSlot& slot = selectorSlots[realIndex];

		slot.characterId = charData->id;
		//slot.characterData = charData;

		slot.characterSelect->onMouseClick.UnsubscribeAll();
		slot.characterSelect->onMouseClick.Subscribe([this, slot]() {UpdateCharacterSlot(slot.characterId); });

		slot.characterName->SetText(charData->name);


		selectorSlots[realIndex].characterName->localVisible = true;

		selectorSlots[realIndex].characterSelect->interactable = true;
		selectorSlots[realIndex].characterSelect->localVisible = true;

	}
}

void InventoryCG::GoToMemebersOffset(int _membersOffset)
{

	if (membersOffset == _membersOffset)
		return;

	prev_pageBtn->isEnabled = true;
	next_pageBtn->isEnabled = true;

	int maxOffset = party->GetMemebersAmount() - membersByPage;

	if (_membersOffset <= 0 && _membersOffset >= maxOffset) {
		membersOffset = 0;
		prev_pageBtn->isEnabled = false;
		next_pageBtn->isEnabled = false;
	}
	else if (_membersOffset <= 0) {
		membersOffset = 0;
		prev_pageBtn->isEnabled = false;
	}
	else if (_membersOffset >= maxOffset) {
		membersOffset = maxOffset;
		next_pageBtn->isEnabled = false;
	}
	else {
		membersOffset = _membersOffset;
	}

	UpdateCharacterSelectorSlots();
}

InventoryCG::UIItemSlots InventoryCG::CreateItemSlot(Inventory* inventory, int index, Item* itemRef, int amount, int textureType)
{

	SDL_Texture* overlay = Engine::Instance().m_assetsDB->GetTexture("item_slot");

	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	UIItemSlots slot;


	int itemAmount = amount;

	slot.index = index;

	slot.itemRef = itemRef;
	slot.amount = itemAmount;
	slot.inventory = inventory;

	Vector2 slotSize = Engine::Instance().m_assetsDB->GetTextureSize(*overlay);
	slotSize.x /= 6;
	slot.itemSelect = new UIButton(*overlay, {0,0}, slotSize, { textureType* (int)slotSize.x,0,(int)slotSize.x, (int)slotSize.y });

	slot.itemAmount = new UITextBox("", *textFont, 16, { 255,255,255,255 }, { 11,15 }, { 32,16 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	slot.itemAmount->SetLocalScale(0.5f);

	slot.itemCoverImage = new UIImage({ 0,0 }, { 20,20 }, { 0,0 }, false, { 0,0,0,0 }, { 0,0,0,50 });
	slot.itemCoverImage->localVisible = false;

	slot.itemImage = new UIImage({ 2,2 }, { 16,16 }, { 0,0 });
	if (itemRef != nullptr) {

		TextureAtlas* atlas = Engine::Instance().m_assetsDB->GetAtlas("items_atlas");
		SDL_Texture* texture = atlas->texture;

		slot.itemImage->SetSprite(*texture, true, atlas->sprites[itemRef->id].rect);


		slot.itemAmount->SetText(to_string(itemAmount));
	}
	else {
		slot.itemImage->localVisible = false;
		slot.itemAmount->localVisible = false;
	}
	slot.itemImage->SetParent(slot.itemSelect);
	slot.itemAmount->SetParent(slot.itemSelect);
	slot.itemCoverImage->SetParent(slot.itemSelect);


	slot.itemSelect->SetParent(container_image);

	return slot;
}

void InventoryCG::UpdateItemSlot(UIItemSlots* slot, Inventory* inventory, Item* itemRef, int amount)
{
	int itemAmount = amount;


	slot->itemRef = itemRef;
	slot->amount = itemAmount;
	slot->inventory = inventory;


	if (!inventory || !itemRef) {
		slot->itemImage->localVisible = false;
		slot->itemAmount->localVisible = false;
	}
	else if (itemRef != nullptr) {

		TextureAtlas* atlas = Engine::Instance().m_assetsDB->GetAtlas("items_atlas");
		SDL_Texture* texture = atlas->texture;

		slot->itemImage->SetSprite(*texture, true, atlas->sprites[itemRef->id].rect);
		slot->itemAmount->SetText(to_string(itemAmount));

		slot->itemImage->localVisible = true;
		slot->itemAmount->localVisible = true;
	}

	
}

void InventoryCG::UpdateItemSlot(UIItemSlots* slot, Inventory* inventory, int index)
{

	const vector<InventorySlot>& items = inventory->GetSlotsData();

	Item* itemRef = nullptr;
	if (items[index].item != nullptr)
		itemRef = items[index].item->GetReference();

	int itemAmount = items[index].count;

	UpdateItemSlot(slot, inventory, itemRef, itemAmount);
}

void InventoryCG::CreateItemSlots()
{

	SDL_Texture* overlay = Engine::Instance().m_assetsDB->GetTexture("item_slot");

	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");


	Vector2 anchor = { 164 ,220 };
	Vector2 slotSize = Engine::Instance().m_assetsDB->GetTextureSize(*overlay);
	slotSize.x /= 6;	
	Vector2 spacing = { 5, 5 };

	int itemsByRow = 8;

	const vector<InventorySlot>& items = playerInventory->GetSlotsData();


	for (int i = 0; i < items.size(); ++i)
	{

		if (itemSlots.size() > i)
			continue;

		int row = i / itemsByRow;
		int col = i % itemsByRow;


		Vector2 offset = { col * (spacing.x+ slotSize.y) ,(slotSize.y + spacing.y) * row };
		Vector2 position = { anchor.x + offset.x, anchor.y + offset.y };

		Item* itemRef = nullptr;
		if (items[i].item != nullptr)
			itemRef = items[i].item->GetReference();

		int itemAmount = items[i].count;

		UIItemSlots slot = CreateItemSlot(playerInventory,i,itemRef,itemAmount);
		slot.itemSelect->SetLocalPosition(position);

		slot.itemSelect->onMouseDown.Subscribe([this, i]() {OnItemSelected(&itemSlots[i]); });
		slot.itemSelect->onMouseOver.Subscribe([this, i]() {OnMouseOverItem(&itemSlots[i]); });
		slot.itemSelect->onMouseExit.Subscribe([this, i]() {OnMouseOverItem(nullptr); });

		itemSlots.emplace_back(slot);
	}
}

void InventoryCG::UpdateItemSlots()
{

	CreateItemSlots();

	for (int i = 0; i < itemSlots.size(); ++i) {
		itemSlots[i].itemImage->localVisible = false;
		itemSlots[i].itemAmount->localVisible = false;
	}


	const vector<InventorySlot>& items = playerInventory->GetSlotsData();


	for (int i = 0; i < items.size(); ++i)
	{
		UIItemSlots& slot = itemSlots[i];



		Item* itemRef = nullptr;
		if (items[i].item != nullptr)
			itemRef = items[i].item->GetReference();

		int itemAmount = items[i].count;

		UpdateItemSlot(&slot, playerInventory, itemRef, itemAmount);

	}
}

void InventoryCG::CreateCharacterSlot()
{
	SDL_Texture* overlay = Engine::Instance().m_assetsDB->GetTexture("party_characterUI");

	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	characterSlot.characterOverlay = new UIImage(*overlay, { 247,88 }, { 66,66 }, { 0,0 }, true, {198,0,66,66});

	characterSlot.characterProfile = new UIImage(*overlay, { 1,1 }, { 64,64 }, { 0,0 });
	characterSlot.characterProfile->localVisible = false;

	characterSlot.characterName = new UITextBox("NO SELECTED", *textFont, 16, {184,132,78,255}, {241,164}, {80 * 2,15 * 2}, {0,0}, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	characterSlot.characterName->SetLocalScale(0.5f);


	characterSlot.characterProfile->SetParent(characterSlot.characterOverlay);

	characterSlot.characterName->SetParent(container_image);
	characterSlot.characterOverlay->SetParent(container_image);

	characterSlot.helmetSlot = CreateItemSlot(nullptr,0,nullptr,0,1);
	characterSlot.helmetSlot.itemSelect->SetLocalPosition({193,99});
	characterSlot.helmetSlot.itemSelect->SetLocalPosition({193,99});

	characterSlot.chestplateSlot = CreateItemSlot(nullptr, 1,nullptr,0,2);
	characterSlot.chestplateSlot.itemSelect->SetLocalPosition({193,151});

	characterSlot.weaponSlot = CreateItemSlot(nullptr, 2,nullptr,0,3);
	characterSlot.weaponSlot.itemSelect->SetLocalPosition({169,125});

	characterSlot.accesorieSlot = CreateItemSlot(nullptr, 3,nullptr,0,4);
	characterSlot.accesorieSlot.itemSelect->SetLocalPosition({217,125});


	characterSlot.helmetSlot.itemSelect->onMouseDown.Subscribe([this]() {OnItemSelected(&characterSlot.helmetSlot); });
	characterSlot.helmetSlot.itemSelect->onMouseOver.Subscribe([this]() {OnMouseOverItem(&characterSlot.helmetSlot); });
	characterSlot.helmetSlot.itemSelect->onMouseExit.Subscribe([this]() {OnMouseOverItem(nullptr); });

	characterSlot.chestplateSlot.itemSelect->onMouseDown.Subscribe([this]() {OnItemSelected(&characterSlot.chestplateSlot); });
	characterSlot.chestplateSlot.itemSelect->onMouseOver.Subscribe([this]() {OnMouseOverItem(&characterSlot.chestplateSlot); });
	characterSlot.chestplateSlot.itemSelect->onMouseExit.Subscribe([this]() {OnMouseOverItem(nullptr); });

	characterSlot.weaponSlot.itemSelect->onMouseDown.Subscribe([this]() {OnItemSelected(&characterSlot.weaponSlot); });
	characterSlot.weaponSlot.itemSelect->onMouseOver.Subscribe([this]() {OnMouseOverItem(&characterSlot.weaponSlot); });
	characterSlot.weaponSlot.itemSelect->onMouseExit.Subscribe([this]() {OnMouseOverItem(nullptr); });

	characterSlot.accesorieSlot.itemSelect->onMouseDown.Subscribe([this]() {OnItemSelected(&characterSlot.accesorieSlot); });
	characterSlot.accesorieSlot.itemSelect->onMouseOver.Subscribe([this]() {OnMouseOverItem(&characterSlot.accesorieSlot); });
	characterSlot.accesorieSlot.itemSelect->onMouseExit.Subscribe([this]() {OnMouseOverItem(nullptr); });
}

void InventoryCG::UpdateCharacterSlot(string charId)
{
	CharacterDatabase::CharacterDefinition* charData = nullptr;
	charData = party->GetCharacterFromMembers(charId);

	TextureAtlas* characterProfilesAtlas = Engine::Instance().m_assetsDB->GetAtlas("character_atlas");
	SDL_Texture* character_profile_texture = nullptr;

	if (charData != nullptr) {
		character_profile_texture = characterProfilesAtlas->texture;
		characterSlot.characterProfile->SetSprite(*character_profile_texture, true, characterProfilesAtlas->sprites[charData->charTemplate->faceId].rect);
		characterSlot.characterProfile->localVisible = true;

		characterSlot.characterOverlay->SetRect({ 0,0,66,66 });

		characterSlot.characterName->SetText(charData->name);

		const vector<InventorySlot>& items = charData->inventory.GetSlotsData();

		Item* item = items[0].item ? items[0].item->GetReference() : nullptr;
		UpdateItemSlot(&characterSlot.helmetSlot, &charData->inventory, item, items[0].count);

		item = items[1].item ? items[1].item->GetReference() : nullptr;
		UpdateItemSlot(&characterSlot.chestplateSlot, &charData->inventory, item, items[1].count);

		item = items[2].item ? items[2].item->GetReference() : nullptr;
		UpdateItemSlot(&characterSlot.weaponSlot, &charData->inventory, item, items[2].count);

		item = items[3].item ? items[3].item->GetReference() : nullptr;
		UpdateItemSlot(&characterSlot.accesorieSlot, &charData->inventory, item, items[3].count);
	}
	else {
		characterSlot.characterProfile->localVisible = false;
		characterSlot.characterOverlay->SetRect({ 198,0,66,66 });
		characterSlot.characterName->SetText("NO SELECTED");

		UpdateItemSlot(&characterSlot.helmetSlot, nullptr, nullptr, 0);
		UpdateItemSlot(&characterSlot.chestplateSlot, nullptr, nullptr, 0);
		UpdateItemSlot(&characterSlot.weaponSlot, nullptr, nullptr, 0);
		UpdateItemSlot(&characterSlot.accesorieSlot, nullptr, nullptr, 0);
	}

	

}

void InventoryCG::CreateExtras()
{

	SDL_Texture* switchPage_texture = Engine::Instance().m_assetsDB->GetTexture("arrow_tex2");

	Mix_Chunk* btn_enter = Engine::Instance().m_assetsDB->GetAudio("btn_enter");
	Mix_Chunk* btn_click = Engine::Instance().m_assetsDB->GetAudio("btn_click");


	itemHoldingImage = new UIImage({ 0,0 }, { 16,16 }, { 0.5f,0.5f });
	itemHoldingImage->localVisible = false;

	/// Nav

	Vector2 arrowSize = Engine::Instance().m_assetsDB->GetTextureSize(*switchPage_texture);

	prev_pageBtn = new UIButton(*switchPage_texture, { 124 ,82 }, arrowSize, { 0,10,17,10 }, { 0,0});
	prev_pageBtn->AddRect(UIButton::ButtonStates::HOVER, { 17,10,17,10 });
	prev_pageBtn->AddRect(UIButton::ButtonStates::PRESSED, { 17*2,10,17,10 });
	prev_pageBtn->AddRect(UIButton::ButtonStates::DISABLED, { 17*3,10,17,10 });
	prev_pageBtn->onMouseClick.Subscribe([this]() {GoToMemebersOffset(membersOffset - 1); });
	prev_pageBtn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	prev_pageBtn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	prev_pageBtn->SetParent(container_image);


	next_pageBtn = new UIButton(*switchPage_texture, { 124 ,336 }, arrowSize, { 0,0,17,10 }, { 0,0 });
	next_pageBtn->AddRect(UIButton::ButtonStates::HOVER, { 17,0,17,10 });
	next_pageBtn->AddRect(UIButton::ButtonStates::PRESSED, { 17 * 2,0,17,10 });
	next_pageBtn->AddRect(UIButton::ButtonStates::DISABLED, { 17 * 3,0,17,10 });
	next_pageBtn->onMouseClick.Subscribe([this]() {GoToMemebersOffset(membersOffset + 1);  });
	next_pageBtn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	next_pageBtn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	next_pageBtn->SetParent(container_image);

	//// RemoveButton
}

void InventoryCG::OnItemSelected(UIItemSlots* slot)
{
	if (slot == nullptr || slot->inventory == nullptr) {
		if (currentHoldedSlot != nullptr) {
			currentHoldedSlot->itemCoverImage->localVisible = false;
			currentHoldedSlot = nullptr;
		}
		itemHoldingImage->localVisible = false;
		return;
	}
	if (currentHoldedSlot == nullptr) {
		if (slot->itemRef != nullptr) {
			currentHoldedSlot = slot;

			TextureAtlas* atlas = Engine::Instance().m_assetsDB->GetAtlas("items_atlas");
			SDL_Texture* texture = atlas->texture;

			itemHoldingImage->SetSprite(*texture, true, atlas->sprites[slot->itemRef->id].rect);
			itemHoldingImage->localVisible = true;
			itemHoldingImage->SetLocalPosition(Engine::Instance().m_input->GetMousePosition() - GetPosition());

			currentHoldedSlot->itemCoverImage->localVisible = true;
		}
	}
	else {
		bool actionDone = false;
		if (currentHoldedSlot->inventory->TryStackItems(currentHoldedSlot->index, slot->index,slot->inventory)) {
			actionDone = true;
		}
		else {
			currentHoldedSlot->inventory->SwapSlots(currentHoldedSlot->index, slot->index, slot->inventory);
			actionDone = true;
		}


		if (actionDone) {

			UpdateItemSlot(currentHoldedSlot, currentHoldedSlot->inventory, currentHoldedSlot->index);
			UpdateItemSlot(slot, slot->inventory, slot->index);

		}

		currentHoldedSlot->itemCoverImage->localVisible = false;
		currentHoldedSlot = nullptr;
		itemHoldingImage->localVisible = false;
	}
}

void InventoryCG::OnMouseOverItem(UIItemSlots* slot)
{
	mouseOverSlot = slot;

}
