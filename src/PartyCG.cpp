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

	prev_pageBtn = new UIButton(*switchPage_texture, { container_image->size.x / 2 ,130 }, { 35,16 }, { 0,0,35,16 }, { 0.5f,0.5f });
	prev_pageBtn->AddRect(UIButton::ButtonStates::HOVER, { 35,0,35,16 });
	prev_pageBtn->AddRect(UIButton::ButtonStates::PRESSED, { 70,0,35,16 });
	prev_pageBtn->AddRect(UIButton::ButtonStates::DISABLED, { 105,0,35,16 });
	prev_pageBtn->onMouseClick.Subscribe([this]() {GoToMemeberPage(membersPage - 1); });
	prev_pageBtn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	prev_pageBtn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	prev_pageBtn->SetParent(container_image);


	next_pageBtn = new UIButton(*switchPage_texture, { container_image->size.x / 2 ,324 }, { 35,16 }, { 0,16,35,16 }, { 0.5f,0.5f });
	next_pageBtn->AddRect(UIButton::ButtonStates::HOVER, { 35,16,35,16 });
	next_pageBtn->AddRect(UIButton::ButtonStates::PRESSED, { 70,16,35,16 });
	next_pageBtn->AddRect(UIButton::ButtonStates::DISABLED, { 105,16,35,16 });
	next_pageBtn->onMouseClick.Subscribe([this]() {GoToMemeberPage(membersPage + 1);  });
	next_pageBtn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	next_pageBtn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	next_pageBtn->SetParent(container_image);


	pageData = new UITextBox("Page: 0/0", *textFont, 16, { 184,132,78,255 }, { 19 ,324 }, { 100, 16 }, { 0,0.5f }, UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment::Middle);
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

void PartyCG::SwitchCharacter(int id)
{
	//if (!switching)
	//{
	//	switching = true;
	//	switchingId = id;
	//	for (size_t i = 0; i < partySlots.size(); i++)
	//	{
	//		if (partySlots[i].characterId == switchingId) {
	//			partySlots[i].switchToggle->SetValue(true,false);
	//			break;
	//		}
	//	}
	//}
	//else {
	//	switching = false;
	//	party->SwapPartyMembers(id, switchingId);
	//	for (size_t i = 0; i < partySlots.size(); i++)
	//	{
	//		partySlots[i].switchToggle->SetValue(false, false);
	//	}
	//	UpdatePartySlots();
	//}
}

void PartyCG::CreatePartySlots()
{
	Vector2 anchor = { 65 ,40 };
	Vector2 slotSize = { 66,66 };
	Vector2 spacing = { 17, 0 };

	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("party_characterUI");
	SDL_Texture* toggle_texture = Engine::Instance().m_assetsDB->GetTexture("toggle_tex2");

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
			slot.characterProfile = new UIImage(*character_profile_texture, { 1,1 }, { 64,64 }, { 0,0 }, true, {0,0,0,0});
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

		slot.characterSelect->SetParent(container_image);

		partySlots.emplace_back(slot);
	}
}

void PartyCG::CreateMemeberSlots()
{
	Vector2 anchor = { container_image->size.x / 2.f ,150 };
	Vector2 slotSize = { 180,77 };
	Vector2 spacing = { 50, 10 };

	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("member_characterUI");
	SDL_Texture* toggle_texture = Engine::Instance().m_assetsDB->GetTexture("toggle_tex2");

	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");


	const vector<CharacterDatabase::CharacterData*> memebers = party->GetMemebers();
	int maxIndex = membersPage * membersByPage + membersByPage;
	if (maxIndex > memebers.size()) {
		maxIndex = memebers.size();
	}

	for (int i = membersPage * membersByPage; i < maxIndex; ++i)
	{
		if (memberSlots.size() > i%4)
			continue;
		CharacterDatabase::CharacterData* charData = memebers[i];

		int row = floor((i) / 2);
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

		slot.charcterName = new UITextBox(charData->name, *textFont, 16, { 184,132,78,255 }, { 74, 4 }, { 101,32 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		slot.charcterName->SetParent(slot.characterSelect);

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

		slot.characterSelect->onMouseClick.UnsubscribeAll();
		slot.characterSelect->onMouseClick.Subscribe([this, id]() {RemovePartyCharacter(id);});

		slot.characterSelect->isEnabled = true;
		partySlots[i].characterProfile->localVisible = true;
		slot.chracterOverlay->isEnabled = true;

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
		maxIndex = memebers.size();
	}

	for (int i = membersPage*membersByPage; i < maxIndex; ++i)
	{
		CharacterDatabase::CharacterData* charData = memebers[i];
		int realIndex = i % 4;
		UIMemberCharacterSlot& slot = memberSlots[realIndex];

		slot.characterId = charData->id;
		slot.charcterLevel->SetText("Lvl: " + to_string(charData->level));
		slot.charcterName->SetText(charData->name);

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

