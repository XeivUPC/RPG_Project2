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

	AddElementToCanvas(container_image);
}

PartyCG::~PartyCG()
{

}

void PartyCG::SwitchCharacter(int id)
{
	if (!switching)
	{
		switching = true;
		switchingId = id;
		for (size_t i = 0; i < slots.size(); i++)
		{
			if (slots[i].characterId == switchingId) {
				slots[i].switchToggle->SetValue(true,false);
				break;
			}
		}
	}
	else {
		switching = false;
		party->SwapPartyMembers(id, switchingId);
		for (size_t i = 0; i < slots.size(); i++)
		{
			slots[i].switchToggle->SetValue(false, false);
		}
		UpdatePartySlots();
	}
}

void PartyCG::CreatePartySlots()
{
	Vector2 anchor = { container_image->size.x / 2.f ,59 };
	Vector2 slotSize = { 180,77 };
	Vector2 spacing = { 50, 20 };

	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("party_characterUI");
	SDL_Texture* toggle_texture = Engine::Instance().m_assetsDB->GetTexture("toggle_tex2");

	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	
	const vector<CharacterDatabase::CharacterData*> partyMembers = party->GetParty();
	for (int i = 0; i < party->GetPartySize(); ++i)
	{
		if (slots.size() > i)
			continue;
		CharacterDatabase::CharacterData* charData = partyMembers[i];
		
		int row = floor((i) / 2);
		int col = (i - 1) % 2;

		Vector2 offset = { (col ? -slotSize.x - spacing.x/2 : spacing.x / 2),(slotSize.y + spacing.y) * row };
		Vector2 position = { anchor.x + offset.x, anchor.y + offset.y };

		UICharacterSlot slot;

		slot.characterId = charData->id;
		slot.characterSelect = new UIButton(position, {249,59}, { 0,0,0,0 }, { 0,0 }, { 0,0,0,0 });

		slot.characterBackground = new UIImage(*bg_texture, { 0,-11 }, slotSize);
		slot.characterBackground->SetParent(slot.characterSelect);

		TextureAtlas* characterProfilesAtlas = Engine::Instance().m_assetsDB->GetAtlas("character_atlas");
		SDL_Texture* character_profile_texture = characterProfilesAtlas->texture;
		slot.characterProfile = new UIImage(*character_profile_texture, { 6,65 }, { 64,64 }, {0,1},true, characterProfilesAtlas->sprites[charData->faceId].rect);
		slot.characterProfile->SetParent(slot.characterSelect);

		slot.charcterName = new UITextBox(charData->name, *textFont, 16, {184,132,78,255}, {74, 4}, { 101,32}, {0,0}, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		slot.charcterName->SetParent(slot.characterSelect);	

		slot.charcterLevel = new UITextBox("Lvl: " + to_string(charData->level), *textFont, 16, { 184,132,78,255 }, { 10, -8 }, { 78,16 }, {0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Top, false);
		slot.charcterLevel->SetParent(slot.characterSelect);
		slot.charcterLevel->SetLocalScale(0.5f);

		slot.addRemoveToggle = new UIToggle(*toggle_texture, { col ? 180 : -16,7 }, { 16,16 }, {16,0,16,16}, { 0,0,16,16 }, { 0,0 }, true);
		slot.addRemoveToggle->SetParent(slot.characterSelect);

		slot.switchToggle = new UIToggle(*toggle_texture, { col ? 180 : -16,23 }, { 16,16 }, {48,0,16,16}, { 32,0,16,16 }, { 0,0 }, false);
		slot.switchToggle->SetParent(slot.characterSelect);
		slot.switchToggle->onValueChange.Subscribe([this, slot]() {SwitchCharacter(slot.characterId); });

		UIImage* barBorder = new UIImage({ 74,39 }, { 101,9 }, {0,0}, false, { 0,0,0,0 }, { 255,255,255,255 });
		barBorder->SetParent(slot.characterSelect);
		slot.hpBar = new UIImage({ 1,1 }, { 99,7 }, {0,0}, false, { 0,0,0,0 }, { 255,0,0,255 });
		slot.hpBar->SetParent(barBorder);
		slot.hpValue = new UITextBox(to_string(charData->health) + "/" + to_string(charData->health), *textFont, 16, { 255,255,255,255 }, { 0,1 }, { 101*2,9*2 }, {0,0}, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		slot.hpValue->SetLocalScale(0.5f);
		slot.hpValue->SetParent(barBorder);

		barBorder = new UIImage({ 74,52 }, { 101,9 }, {0,0}, false, { 0,0,0,0 }, { 255,255,255,255 });
		barBorder->SetParent(slot.characterSelect);
		slot.energyBar = new UIImage({ 1,1 }, {99 ,7 }, {0,0}, false, { 0,0,0,0 }, { 110,182,255,255 });
		slot.energyBar->SetParent(barBorder);
		slot.energyValue = new UITextBox(to_string(charData->health) + "/" + to_string(charData->health), *textFont, 16, { 255,255,255,255 }, { 0,1 }, { 101 * 2,9 * 2 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
		slot.energyValue->SetLocalScale(0.5f);
		slot.energyValue->SetParent(barBorder);

		slot.characterSelect->SetParent(container_image);

		slots.emplace_back(slot);
	}
}


void PartyCG::UpdatePartySlots()
{
	CreatePartySlots();
	for (int i = 0; i < slots.size(); ++i) {
		slots[i].characterSelect->localVisible = false;
	}

	const vector<CharacterDatabase::CharacterData*> partyMembers = party->GetParty();
	for (int i = 0; i < party->GetPartySize(); ++i)
	{
		CharacterDatabase::CharacterData* charData = partyMembers[i];
		UICharacterSlot& slot = slots[i];

		slot.characterId = charData->id;
		slot.charcterLevel->SetText("Lvl: " + to_string(charData->level));
		slot.charcterName->SetText(charData->name);

		TextureAtlas* characterProfilesAtlas = Engine::Instance().m_assetsDB->GetAtlas("character_atlas");
		SDL_Texture* character_profile_texture = characterProfilesAtlas->texture;
		slot.characterProfile->SetSprite(*character_profile_texture,true, characterProfilesAtlas->sprites[charData->faceId].rect);

		//// EditLive
		slot.hpValue->SetText(to_string(charData->health) + "/" + to_string(charData->health));
		slot.energyValue->SetText(to_string(charData->health) + "/" + to_string(charData->health));

		////EditSwitchToggle
		slot.switchToggle->onValueChange.UnsubscribeAll();
		slot.switchToggle->onValueChange.Subscribe([this, slot]() {SwitchCharacter(slot.characterId); });


		slot.characterSelect->localVisible = true;
	}
}

void PartyCG::ChangePartyToTrack(Party* partyToTrack)
{
	party = partyToTrack;
	UpdatePartySlots();
}

void PartyCG::Reset()
{
	switching = false;
	switchingId = -1;
}

