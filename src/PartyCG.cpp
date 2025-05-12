#include "PartyCG.h"
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


#include "Globals.h"
#include <algorithm>

PartyCG::PartyCG()
{
	///// AssetsLoading
	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	Mix_Chunk* btn_enter = Engine::Instance().m_assetsDB->GetAudio("btn_enter");
	Mix_Chunk* btn_click = Engine::Instance().m_assetsDB->GetAudio("btn_click");

	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("uiBox4");
	SDL_Texture* close_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex2");
	SDL_Texture* switchPage_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex4");

	///// Elements

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*bg_texture);
	container_image = new UIImage(*bg_texture, { 0,0 }, textureSize, { 0,0 });

	SetPosition(Vector2{ LOGIC_SCREEN_WIDTH / 2 - (float)textureSize.x / 2,LOGIC_SCREEN_HEIGHT / 2 - (float)textureSize.y / 2 });


	UITextBox* boxTitle_text = new UITextBox("Party Characters", *textFont, 32, { 184,132,78,255 }, { textureSize.x / 2 , 0 }, { textureSize.x * 2, 50 }, { 0.5f,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	boxTitle_text->SetLocalScale(0.75f);
	boxTitle_text->SetParent(container_image);


	UIButton* close_btn = new UIButton(*close_texture, { 8, 8 }, { 15,15 }, { 0,0,15,15 }, { 0,0 });
	close_btn->AddRect(UIButton::ButtonStates::HOVER, { 15,0,15,15 });
	close_btn->AddRect(UIButton::ButtonStates::PRESSED, { 30,0,15,15 });
	close_btn->onMouseClick.Subscribe([this]() {isVisible = false; });
	close_btn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	close_btn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	close_btn->SetParent(container_image);

	prev_pageBtn = new UIButton(*switchPage_texture, { container_image->size.x / 2 ,150 }, { 35,16 }, { 0,0,35,16 }, { 0.5f,0.5f });
	prev_pageBtn->AddRect(UIButton::ButtonStates::HOVER, { 35,0,35,16 });
	prev_pageBtn->AddRect(UIButton::ButtonStates::PRESSED, { 70,0,35,16 });
	prev_pageBtn->AddRect(UIButton::ButtonStates::DISABLED, { 105,0,35,16 });
	prev_pageBtn->onMouseClick.Subscribe([this]() {GoToMemeberPage(membersPage - 1); });
	prev_pageBtn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	prev_pageBtn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	prev_pageBtn->SetParent(container_image);


	next_pageBtn = new UIButton(*switchPage_texture, { container_image->size.x / 2 ,345 }, { 35,16 }, { 0,16,35,16 }, { 0.5f,0.5f });
	next_pageBtn->AddRect(UIButton::ButtonStates::HOVER, { 35,16,35,16 });
	next_pageBtn->AddRect(UIButton::ButtonStates::PRESSED, { 70,16,35,16 });
	next_pageBtn->AddRect(UIButton::ButtonStates::DISABLED, { 105,16,35,16 });
	next_pageBtn->onMouseClick.Subscribe([this]() {GoToMemeberPage(membersPage + 1);  });
	next_pageBtn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	next_pageBtn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	next_pageBtn->SetParent(container_image);


	pageData = new UITextBox("Page: 0/0", *textFont, 16, { 184,132,78,255 }, { 19 ,345 }, { 100, 16 }, { 0,0.5f }, UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment::Middle);
	pageData->SetParent(container_image);

	AddElementToCanvas(container_image);
}

PartyCG::~PartyCG()
{

}

void PartyCG::RemovePartyCharacter(int id)
{
	party->RemovePartyMemeber(id);
	UpdateMemberSlots();
	UpdatePartySlots();
}

void PartyCG::AddPartyCharacter(int id)
{
	party->AddPartyMemeber(id);
	UpdateMemberSlots();
	UpdatePartySlots();
}

void PartyCG::SwitchCharacter(int slot1, int slot2)
{
	party->SwapPartyMembers(partySlots[slot1].characterId, partySlots[slot2].characterId);
	UpdatePartySlots();
}

void PartyCG::CreatePartySlots()
{
	Vector2 anchor = { 65 ,30 };
	Vector2 slotSize = { 66,66 };
	Vector2 spacing = { 17, 0 };

	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("party_characterUI");
	SDL_Texture* toggle_texture = Engine::Instance().m_assetsDB->GetTexture("toggle_tex2");
	SDL_Texture* options_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex5");

	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	/// Modify
	const vector<CharacterDatabase::CharacterData*> partyMembers = party->GetParty();
	for (int i = 0; i < party->GetMaxPartySize(); ++i)
	{
		if (partySlots.size() > i)
			continue;

		CharacterDatabase::CharacterData* charData =nullptr;
		UIPartyCharacterSlot slot;

		if (partyMembers.size() > i) {
			charData = partyMembers[i];
			slot.characterId = charData->id;
		}else
			slot.characterId = INT8_MAX;
		
		Vector2 offset = { (slotSize.x+spacing.x)*i , 0 };
		Vector2 position = { anchor.x + offset.x, anchor.y + offset.y };

		
		slot.characterSelect = new UIButton(position, { 66,66 }, { 0,0,0,0 }, { 0,0 }, {0,0,0,0});
		int id = slot.characterId;
		slot.characterSelect->onMouseClick.Subscribe([this, id]() {RemovePartyCharacter(id);});
		if (charData == nullptr)
			slot.characterSelect->isEnabled = false;


		TextureAtlas* characterProfilesAtlas = Engine::Instance().m_assetsDB->GetAtlas("character_atlas");
		SDL_Texture* character_profile_texture = nullptr;
		if (charData != nullptr) {
			character_profile_texture = characterProfilesAtlas->texture;
			slot.characterProfile = new UIImage(*character_profile_texture, { 1,1 }, { 64,64 }, { 0,0 }, true, characterProfilesAtlas->sprites[charData->faceId].rect);
		}
		else {
			slot.characterProfile = new UIImage({ 1,1 }, { 64,64 }, { 0,0 }, true, {0,0,0,0});
		}
		slot.characterProfile->SetParent(slot.characterSelect);
		if (charData == nullptr)
			slot.characterProfile->localVisible = false;


		slot.chracterOverlay = new UIButton(*bg_texture, {0,0}, { 66,66 }, { 0,0,66,66 }, { 0,0 });
		slot.chracterOverlay->AddRect(UIButton::ButtonStates::HOVER, { 66,0,66,66 });
		slot.chracterOverlay->AddRect(UIButton::ButtonStates::PRESSED, { 66*2,0,66,66 });
		slot.chracterOverlay->AddRect(UIButton::ButtonStates::DISABLED, { 66 * 3,0,66,66 });
		slot.chracterOverlay->SetParent(slot.characterSelect);
		if (charData == nullptr)
			slot.chracterOverlay->isEnabled = false;


		string name = "";
		if (charData != nullptr)
			name = charData->name;
		slot.characterName = new UITextBox(name, *textFont, 16, { 184,132,78,255 }, { -7, 69 }, { 80*2,15*2}, {0,0}, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		slot.characterName->SetParent(slot.characterSelect);
		slot.characterName->SetLocalScale(0.5f);
		if (charData == nullptr)
			slot.characterName->localVisible = false;


		slot.moveLeftBtn = new UIButton(*options_texture, { 1,85 }, { 12,12 }, { 12,0,12,12 }, { 0,0 });
		slot.moveLeftBtn->AddRect(UIButton::ButtonStates::HOVER, { 12,12,12,12 });
		slot.moveLeftBtn->AddRect(UIButton::ButtonStates::PRESSED, { 12,12*2,12,12 });
		slot.moveLeftBtn->AddRect(UIButton::ButtonStates::DISABLED, { 12,12*3,12,12 });
		slot.moveLeftBtn->SetParent(slot.characterSelect);
		if (charData == nullptr || i == 0)
			slot.moveLeftBtn->isEnabled = false;
		else
			slot.moveLeftBtn->onMouseClick.Subscribe([this, i]() {SwitchCharacter(i, i - 1); });

		slot.searchBtn = new UIButton(*options_texture, { 27,85 }, { 12,12 }, { 0,0,12,12 }, { 0,0 });
		slot.searchBtn->AddRect(UIButton::ButtonStates::HOVER, { 0,12,12,12 });
		slot.searchBtn->AddRect(UIButton::ButtonStates::PRESSED, { 0,12*2,12,12 });
		slot.searchBtn->AddRect(UIButton::ButtonStates::DISABLED, { 0,12*3,12,12 });
		slot.searchBtn->SetParent(slot.characterSelect);
		if (charData == nullptr)
			slot.searchBtn->isEnabled = false;
		else
			slot.searchBtn->onMouseClick.Subscribe([this, id]() {GoToMemeberPageByCharacterId(id); });

		slot.moveRightBtn = new UIButton(*options_texture, { 53,85 }, { 12,12 }, { 12*2,0,12,12 }, { 0,0 });
		slot.moveRightBtn->AddRect(UIButton::ButtonStates::HOVER, { 12*2,12,12,12 });
		slot.moveRightBtn->AddRect(UIButton::ButtonStates::PRESSED, { 12 * 2,12*2,12,12 });
		slot.moveRightBtn->AddRect(UIButton::ButtonStates::DISABLED, { 12 * 2,12*3,12,12 });
		slot.moveRightBtn->SetParent(slot.characterSelect);
		if (charData == nullptr || i == partyMembers.size() - 1)
			slot.moveRightBtn->isEnabled = false;
		else
			slot.moveRightBtn->onMouseClick.Subscribe([this, i]() {SwitchCharacter(i, i + 1); });

		slot.characterSelect->SetParent(container_image);

		partySlots.emplace_back(slot);
	}
}

void PartyCG::CreateMemeberSlots()
{
	Vector2 anchor = { container_image->size.x / 2.f ,170 };
	Vector2 slotSize = { 180,77 };
	Vector2 spacing = { 50, 10 };

	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("member_characterUI");
	SDL_Texture* toggle_texture = Engine::Instance().m_assetsDB->GetTexture("toggle_tex2");

	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");


	const vector<CharacterDatabase::CharacterData*> memebers = party->GetMemebers();
	int maxIndex = membersPage * membersByPage + membersByPage;
	if (maxIndex > memebers.size()) {
		maxIndex = (int)memebers.size();
	}

	for (int i = membersPage * membersByPage; i < maxIndex; ++i)
	{
		if (memberSlots.size() > i%4)
			continue;
		CharacterDatabase::CharacterData* charData = memebers[i];

		int row = (int)(floor((i) / 2));
		int col = (i - 1) % 2;

		Vector2 offset = { (col ? -slotSize.x - spacing.x / 2 : spacing.x / 2),(slotSize.y + spacing.y) * row };
		Vector2 position = { anchor.x + offset.x, anchor.y + offset.y };

		UIMemberCharacterSlot slot;

		slot.characterId = charData->id;
		slot.characterSelect = new UIButton(position, { 249,59 }, { 0,0,0,0 }, { 0,0 }, { 0,0,0,0 });

		slot.characterBackground = new UIImage(*bg_texture, { 0,-11 }, slotSize);
		slot.characterBackground->SetParent(slot.characterSelect);

		TextureAtlas* characterProfilesAtlas = Engine::Instance().m_assetsDB->GetAtlas("character_atlas");
		SDL_Texture* character_profile_texture = characterProfilesAtlas->texture;
		slot.characterProfile = new UIImage(*character_profile_texture, { 6,65 }, { 64,64 }, { 0,1 }, true, characterProfilesAtlas->sprites[charData->faceId].rect);
		slot.characterProfile->SetParent(slot.characterSelect);

		slot.characterName = new UITextBox(charData->name, *textFont, 16, { 184,132,78,255 }, { 74, 4 }, { 101,32 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		slot.characterName->SetParent(slot.characterSelect);

		slot.charcterLevel = new UITextBox("Lvl: " + to_string(charData->level), *textFont, 16, { 184,132,78,255 }, { 10, -8 }, { 78,16 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Top, false);
		slot.charcterLevel->SetParent(slot.characterSelect);
		slot.charcterLevel->SetLocalScale(0.5f);

		slot.addRemoveToggle = new UIToggle(*toggle_texture, { col ? 180 : -16,7 }, { 16,16 }, { 16,0,16,16 }, { 0,0,16,16 }, { 0,0 }, true);
		slot.addRemoveToggle->SetParent(slot.characterSelect);

		UIImage* barBorder = new UIImage({ 74,39 }, { 101,9 }, { 0,0 }, false, { 0,0,0,0 }, { 255,255,255,255 });
		barBorder->SetParent(slot.characterSelect);
		slot.hpBar = new UIImage({ 1,1 }, { 99,7 }, { 0,0 }, false, { 0,0,0,0 }, { 255,0,0,255 });
		slot.hpBar->SetParent(barBorder);
		slot.hpValue = new UITextBox(to_string(charData->health) + "/" + to_string(charData->health), *textFont, 16, { 255,255,255,255 }, { 0,1 }, { 101 * 2,9 * 2 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		slot.hpValue->SetLocalScale(0.5f);
		slot.hpValue->SetParent(barBorder);

		barBorder = new UIImage({ 74,52 }, { 101,9 }, { 0,0 }, false, { 0,0,0,0 }, { 255,255,255,255 });
		barBorder->SetParent(slot.characterSelect);
		slot.energyBar = new UIImage({ 1,1 }, { 99 ,7 }, { 0,0 }, false, { 0,0,0,0 }, { 110,182,255,255 });
		slot.energyBar->SetParent(barBorder);
		slot.energyValue = new UITextBox(to_string(charData->health) + "/" + to_string(charData->health), *textFont, 16, { 255,255,255,255 }, { 0,1 }, { 101 * 2,9 * 2 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		slot.energyValue->SetLocalScale(0.5f);
		slot.energyValue->SetParent(barBorder);

		slot.characterSelect->SetParent(container_image);

		memberSlots.emplace_back(slot);
	}
}

void PartyCG::UpdatePartySlots()
{
	/// Modify

	CreatePartySlots();
	for (int i = 0; i < partySlots.size(); ++i) {
		partySlots[i].characterSelect->isEnabled = false;
		partySlots[i].characterProfile->localVisible = false;
		partySlots[i].chracterOverlay->isEnabled = false;
		partySlots[i].moveLeftBtn->isEnabled = false;
		partySlots[i].searchBtn->isEnabled = false;
		partySlots[i].moveRightBtn->isEnabled = false;
		partySlots[i].characterName->localVisible = false;

	}

	const vector<CharacterDatabase::CharacterData*> partyMembers = party->GetParty();
	for (int i = 0; i < party->GetPartySize(); ++i)
	{
		CharacterDatabase::CharacterData* charData = partyMembers[i];
		UIPartyCharacterSlot& slot = partySlots[i];

		slot.characterId = charData->id;
		int id = slot.characterId;

		TextureAtlas* characterProfilesAtlas = Engine::Instance().m_assetsDB->GetAtlas("character_atlas");
		SDL_Texture* character_profile_texture = characterProfilesAtlas->texture;
		slot.characterProfile->SetSprite(*character_profile_texture, true, characterProfilesAtlas->sprites[charData->faceId].rect);
		slot.characterName->SetText(charData->name);

		slot.characterSelect->onMouseClick.UnsubscribeAll();
		slot.characterSelect->onMouseClick.Subscribe([this, id]() {RemovePartyCharacter(id);});

		slot.characterSelect->isEnabled = true;
		partySlots[i].characterProfile->localVisible = true;
		slot.chracterOverlay->isEnabled = true;
		slot.characterName->localVisible = true;


		if (i != 0) {
			slot.moveLeftBtn->isEnabled = true;
			slot.moveLeftBtn->onMouseClick.UnsubscribeAll();
			slot.moveLeftBtn->onMouseClick.Subscribe([this, i]() {SwitchCharacter(i, i - 1); });
		}

		slot.searchBtn->isEnabled = true;
		slot.searchBtn->onMouseClick.UnsubscribeAll();
		slot.searchBtn->onMouseClick.Subscribe([this, id]() {GoToMemeberPageByCharacterId(id); });

		if (i != party->GetPartySize() - 1) {
			slot.moveRightBtn->isEnabled = true;
			slot.moveRightBtn->onMouseClick.UnsubscribeAll();
			slot.moveRightBtn->onMouseClick.Subscribe([this, i]() {SwitchCharacter(i, i + 1); });
		}

	}
}

void PartyCG::UpdateMemberSlots()
{
	CreateMemeberSlots();
	for (int i = 0; i < memberSlots.size(); ++i) {
		memberSlots[i].characterSelect->localVisible = false;
	}
	const vector<CharacterDatabase::CharacterData*> memebers = party->GetMemebers();

	int maxIndex = membersPage * membersByPage + membersByPage;
	if (maxIndex > memebers.size()) {
		maxIndex = (int)memebers.size();
	}

	for (int i = membersPage*membersByPage; i < maxIndex; ++i)
	{
		CharacterDatabase::CharacterData* charData = memebers[i];
		int realIndex = i % 4;
		UIMemberCharacterSlot& slot = memberSlots[realIndex];

		slot.characterId = charData->id;
		slot.charcterLevel->SetText("Lvl: " + to_string(charData->level));
		slot.characterName->SetText(charData->name);

		TextureAtlas* characterProfilesAtlas = Engine::Instance().m_assetsDB->GetAtlas("character_atlas");
		SDL_Texture* character_profile_texture = characterProfilesAtlas->texture;
		slot.characterProfile->SetSprite(*character_profile_texture, true, characterProfilesAtlas->sprites[charData->faceId].rect);

		//// EditLive
		slot.hpValue->SetText(to_string(charData->health) + "/" + to_string(charData->health));
		slot.energyValue->SetText(to_string(charData->health) + "/" + to_string(charData->health));

		bool isInParty = party->IsMemberInParty(slot.characterId);
		slot.addRemoveToggle->SetValue(!isInParty, false);

		slot.addRemoveToggle->onValueChange.UnsubscribeAll();
		slot.addRemoveToggle->onValueChange.Subscribe([this, slot]() {!slot.addRemoveToggle->IsOn() ? AddPartyCharacter(slot.characterId) : RemovePartyCharacter(slot.characterId); });

		////EditSwitchToggle
		//slot.switchToggle->onValueChange.UnsubscribeAll();
		//slot.switchToggle->onValueChange.Subscribe([this, slot]() {SwitchCharacter(slot.characterId); });


		slot.characterSelect->localVisible = true;
	}
}

void PartyCG::GoToMemeberPage(int page)
{
	if (page == membersPage)
		return;

	prev_pageBtn->isEnabled = true;
	next_pageBtn->isEnabled = true;

	int maxPage = party->GetMemebersAmount() / membersByPage;

	if (page <= 0 && page >= maxPage) {
		membersPage = 0;
		prev_pageBtn->isEnabled = false;
		next_pageBtn->isEnabled = false;
	}
	else if (page <= 0) {
		membersPage = 0;
		prev_pageBtn->isEnabled = false;
	}
	else if (page >= maxPage) {
		membersPage = maxPage;
		next_pageBtn->isEnabled = false;
	}
	else {
		membersPage = page;
	}

	pageData->SetText("Page: " + to_string(membersPage + 1) + "/" + to_string(maxPage + 1));

	UpdateMemberSlots();
}

void PartyCG::GoToMemeberPageByCharacterId(int id)
{
	const vector<CharacterDatabase::CharacterData*> memebers = party->GetMemebers();

	auto it = find_if(memebers.begin(), memebers.end(), [id](const CharacterDatabase::CharacterData* slot) {
		return slot->id == id;
		});

	if (it != memebers.end()) {
		int index = (int)distance(memebers.begin(), it);
		GoToMemeberPage(index/ membersByPage);
		return;
	}
}

void PartyCG::ChangePartyToTrack(Party* partyToTrack)
{
	party = partyToTrack;
	UpdatePartySlots();
	GoToMemeberPage(0);
}

void PartyCG::Reset()
{
	switching = false;
	switchingId = -1;
	membersPage = -1;
}

