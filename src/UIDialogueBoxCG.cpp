#include "UIDialogueBoxCG.h"

#include "GameScene.h"
#include "PlayerCharacter.h"
#include "CombatSystem.h"
#include "Inventory.h"
#include "ItemList.h"
#include "Party.h"

#include "MissionManager.h"
#include "MissionList.h"
#include "MissionHolder.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "CharacterDatabase.h"
#include "ModuleTime.h"
#include "TextureAtlas.h"
#include "DialogueSystem.h"
#include "DrawingTools.h"

#include "UIImage.h"
#include "UIButton.h"
#include "UITextBox.h"

#include "Globals.h"

#include <sstream>
#include <charconv>

UIDialogueBoxCG::UIDialogueBoxCG(DialogueSystem* _dialogueSystem)
{
	SDL_Texture* tex = Engine::Instance().m_assetsDB->GetTexture("dialogue_box");
	SDL_Texture* tex2 = Engine::Instance().m_assetsDB->GetTexture("dialogue_answerBox");
	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("alagard");

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*tex);
	dialogueTextBox = new UIImage(*tex, { 0,0 }, textureSize);
	dialogueChoiceBox = new UIImage(*tex2, { 0,0 }, textureSize);
	AddElementToCanvas(dialogueTextBox);
	AddElementToCanvas(dialogueChoiceBox);

	contentTextBox = new UITextBox("", *font, 16, { 184,132,78,255 }, { 16,291 }, { 586,58 }, {0,0}, UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment::Top, true);
	AddElementToCanvas(contentTextBox);

	characterNameTextBox = new UITextBox("", *font, 16, { 184,132,78,255 }, { 6, 263 }, { 230,18 }, { 0,0 }, UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment::Middle, true);
	AddElementToCanvas(characterNameTextBox);

	UIButton* btn = new UIButton(*tex, { 0,279 }, { LOGIC_SCREEN_WIDTH,80 }, { 0,0,0,0 }, { 0,0 });
	btn->onMouseClick.Subscribe([this]() {NextDialogue(); });
	btn->SetLocalScale(1);
	AddElementToCanvas(btn);


	characterPortraitImage = new UIImage({ 0,261 }, { 64, 64 }, { 0,1 }, true, { 0,0,64,64 }, {255,255,255,255});
	characterPortraitImage->SetLocalScale(1.5f);
	AddElementToCanvas(characterPortraitImage);



	btns.emplace_back(btn);

	dialogue = _dialogueSystem;

	dialogue->onSignalCall.Subscribe([this](Signal* signal) {SignalReader(signal); });
	dialogue->onDialogLoaded.Subscribe([this]() {SetVariablesOnStart(); });
	dialogue->onDialogNodeChange.Subscribe([this]() {ChangeDialogueNode(); });
	dialogue->onDialogEnd.Subscribe([this]() {EndDialogue(); });

	dialogue->LoadDialogueWorkspace("Assets/Dialogues/Workspaces/RPG_Game.json");
}

void UIDialogueBoxCG::UpdateCanvas()
{

	dialogue->Update();
	if (dialogue->IsDialogueActive()) {
		dialogueTextBox->GetColor().a = 255;
		const DialogueNode& node = dialogue->GetCurrentDialogue();

		if (typewriterMode) {
			typewriterTimer.Step(ModuleTime::deltaTime);
			if (typewriterText != node.text) {
				if (typewriterTimer.ReadSec() > typewriterSpeed) {
					typewriterTimer.Start();
					char c = node.text.at(typewriterText.size());
					typewriterText += c;
				}
			}
			contentTextBox->SetText(typewriterText);
		}
		else {

			contentTextBox->SetText(node.text);
		}

		if (typewriterMode && typewriterText != node.text) {
			btns[0]->isEnabled = true;
			dialogueChoiceBox->GetColor().a = 0;
			while (btns.size() != 1) {
				RemoveElementFromCanvas(btns.back());
				delete btns.back();
				btns.pop_back();
			}
		}
		else if (node.choices.size() > 1)
		{

			btns[0]->isEnabled = false;
			dialogueChoiceBox->GetColor().a = 255;

			if (node.choices.size() == ((int)btns.size()) - 1) {
			}
			else {	
				while (btns.size() != 1) {
					RemoveElementFromCanvas(btns.back());
					delete btns.back();
					btns.pop_back();
				}
				for (int i = 1; i < node.choices.size() + 1; i++)
				{
					CreateChoiceButton(node.choices[i - 1].text, i - 1, 3);
				}
			}
			
		}
		else {

			while (btns.size() > 1) {
				RemoveElementFromCanvas(btns.back());
				delete btns.back();
				btns.pop_back();
			}

			btns[0]->isEnabled = true;
			dialogueChoiceBox->GetColor().a = 0;
		}
		
		
		characterNameTextBox->SetText(node.character.name);
	}

	UICanvas::UpdateCanvas();
}

void UIDialogueBoxCG::CreateChoiceButton(string text, int index, float verticalSpacing)
{
	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("alagard");

	UITextBox* textBox = new UITextBox(text, *font, 16, { 184,132,78,255 }, { 0,2 }, { 235,32 }, { 0,0}, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle, true);
	Vector2Int textSize = textBox->ProccesTextSize();
	textBox->size.y = textSize.y;

	int verticalPositionToPlace = (int)(123 + verticalSpacing);
	if (index != 0) {
		verticalPositionToPlace = (int)(btns[index]->GetPosition().y + btns[index]->size.y + verticalSpacing);
	}

	UIButton* btn = new UIButton({ 388 ,(int)(verticalPositionToPlace)}, { 235,textSize.y }, { 0,0,0,0 }, { 0,0 }, {148,112,75,255});

	btn->onMouseClick.Subscribe([this, index]() {dialogue->ProcessInput(index); });

	textBox->SetParent(btn);

	AddElementToCanvas(btn);

	btns.emplace_back(btn);
}

void UIDialogueBoxCG::ChangeDialogueNode()
{

	typewriterText = "";

	while (btns.size() != 1) {
		RemoveElementFromCanvas(btns.back());
		delete btns.back();
		btns.pop_back();
	}


	const DialogueNode& node = dialogue->GetCurrentDialogue();
	characterPortraitImage->SetColor({ 255, 255, 255, 0 });
	for (size_t i = 0; i < node.character.portraits.size(); i++)
	{
		if (node.character.portraits[i].id.find(node.portraitId) != std::string::npos)
		{
			TextureAtlas* atlas = Engine::Instance().m_assetsDB->GetAtlas("character_atlas");

			SDL_Texture* tex = atlas->texture;

			characterPortraitImage->SetSprite(*tex, true, atlas->sprites[node.character.portraits[i].name].rect);
			characterPortraitImage->SetColor({ 255, 255, 255, 255 });
			break;
		}
	}
}

void UIDialogueBoxCG::EndDialogue()
{
	characterPortraitImage->SetColor({ 255, 255, 255, 0 });
	dialogueTextBox->GetColor().a = 0;
	contentTextBox->SetText("");
	characterNameTextBox->SetText("");
}

void UIDialogueBoxCG::NextDialogue()
{
	if (typewriterMode) {
		if (typewriterText != dialogue->GetCurrentDialogue().text) {
			typewriterText = dialogue->GetCurrentDialogue().text;
		}
		else {
			dialogue->ProcessInput(0);
		}
	}
	else {
		dialogue->ProcessInput(0);
	}
}


void UIDialogueBoxCG::SetVariablesOnStart()
{
	CharacterDatabase& databaseCharacters = CharacterDatabase::Instance();

	for (const auto& character : databaseCharacters.GetCharacters())
	{
		dialogue->AddGameStateVariable("Char" + to_string(character.second.id) + "_State", (float)character.second.state);
		dialogue->AddGameStateVariable("Char" + to_string(character.second.id) + "_Friendship", (float)character.second.friendShip);
		dialogue->AddGameStateVariable("Char" + to_string(character.second.id) + "_Love", (float)character.second.love);
	}
	
}

void UIDialogueBoxCG::SignalReader(Signal* signal)
{
	if (signal->name == "ChangeTextColor") {
		if (holds_alternative<string>(signal->data)) {
			vector<int> colorsData;
			string color = get<string>(signal->data);
			color.erase(remove(color.begin(), color.end(), ' '));
			stringstream ss(color);
			string temp;

			while (getline(ss, temp, ','))
			{
				colorsData.emplace_back(stoi(temp));
			}
			contentTextBox->SetColor({ (unsigned char)colorsData[0], (unsigned char)colorsData[1], (unsigned char)colorsData[2], (unsigned char)colorsData[3] });
		}
	}
	else if (signal->name == "SetTypewriterMode") {
		if (holds_alternative<float>(signal->data)) {
			typewriterMode = (bool)((int)get<float>(signal->data));
		}
	}
	else if (signal->name == "SetTypewriterSpeed") {
		if (holds_alternative<float>(signal->data)) {
			typewriterSpeed = get<float>(signal->data);
		}
	}
	else if (signal->name == "SetNpcStatusByID") {
		if (holds_alternative<Vector2>(signal->data)) {
			Vector2 data = get<Vector2>(signal->data);
			CharacterDatabase::Instance().GetCharacterData((int)data.x).state = (int)data.y;
			dialogue->AddGameStateVariable("Char" + to_string((int)data.x) + "_State", data.y);
		}
	}
	else if (signal->name == "SetNpcFriendshipByID") {
		if (holds_alternative<Vector2>(signal->data)) {
			Vector2 data = get<Vector2>(signal->data);
			CharacterDatabase::Instance().GetCharacterData((int)data.x).friendShip = (int)data.y;
			dialogue->AddGameStateVariable("Char" + to_string((int)data.x) + "_Friendship", data.y);
		}
	}
	else if (signal->name == "AddNpcFriendshipByID") {
		if (holds_alternative<Vector2>(signal->data)) {
			Vector2 data = get<Vector2>(signal->data);
			CharacterDatabase::Instance().GetCharacterData((int)data.x).friendShip += (int)data.y;
			dialogue->AddGameStateVariable("Char" + to_string((int)data.x) + "_Friendship", data.y);
		}
	}
	else if (signal->name == "SetNpcLoveByID") {
		if (holds_alternative<Vector2>(signal->data)) {
			Vector2 data = get<Vector2>(signal->data);
			CharacterDatabase::Instance().GetCharacterData((int)data.x).love = (int)data.y;
			dialogue->AddGameStateVariable("Char" + to_string((int)data.x) + "_Love", data.y);
		}
	}
	else if (signal->name == "AddNpcLoveByID") {
		if (holds_alternative<Vector2>(signal->data)) {
			Vector2 data = get<Vector2>(signal->data);
			CharacterDatabase::Instance().GetCharacterData((int)data.x).love += (int)data.y;
			dialogue->AddGameStateVariable("Char" + to_string((int)data.x) + "_Love", data.y);
		}
	}
	else if (signal->name == "AddItem") {
		if (holds_alternative<string>(signal->data)) {
			string data = get<string>(signal->data);

			size_t dash_pos = data.find('-');
			string name = data.substr(0, dash_pos);
			int amount = stoi(data.substr(dash_pos + 1));

			Item* itemRef = ItemList::Instance().ItemByID(name);
			Engine::Instance().s_game->GetPlayer()->inventory->AddItem(*(new InventoryItem(itemRef)), amount);
		}
	}
	else if (signal->name == "RemoveItem") {
		if (holds_alternative<string>(signal->data)) {
			string data = get<string>(signal->data);

			size_t dash_pos = data.find('-');
			string name = data.substr(0, dash_pos);
			int amount = stoi(data.substr(dash_pos + 1));

			Engine::Instance().s_game->GetPlayer()->inventory->RemoveItem(name, amount);
		}
	}
	else if (signal->name == "CheckIfHasItem") {
		if (holds_alternative<string>(signal->data)) {
			string data = get<string>(signal->data);

			size_t dash_pos = data.find('-');
			string name = data.substr(0, dash_pos);
			int amount = stoi(data.substr(dash_pos + 1));

			dialogue->AddGameStateVariable("HasItem", Engine::Instance().s_game->GetPlayer()->inventory->HasItem(name, amount));
		}
	}
	else if (signal->name == "CheckIfCanGiveItem") {
		if (holds_alternative<string>(signal->data)) {
			string data = get<string>(signal->data);

			size_t dash_pos = data.find('-');
			string name = data.substr(0, dash_pos);
			int amount = stoi(data.substr(dash_pos + 1));

			dialogue->AddGameStateVariable("CanGiveItem", Engine::Instance().s_game->GetPlayer()->inventory->CanAddItem(name, amount));
		}
	}
	else if (signal->name == "AddMission") {
		if (holds_alternative<string>(signal->data)) {
			string data = get<string>(signal->data);

			MissionManager::Instance().AddMission(*new MissionHolder(MissionList::Instance().MissionByID(data)));
		}
	}
	else if (signal->name == "RemoveMission") {
		if (holds_alternative<string>(signal->data)) {
			string data = get<string>(signal->data);

			MissionManager::Instance().RemoveMission(data);
		}
		}
	else if (signal->name == "CheckIfMissionCompleted") {
		if (holds_alternative<string>(signal->data)) {
			string data = get<string>(signal->data);
			if (!MissionManager::Instance().HasMission(data))
				dialogue->AddGameStateVariable("HasCompletedMission", MissionManager::Instance().IsMissionCompleted(data));
			
		}
	}
	else if (signal->name == "UnlockNpc") {
		if (holds_alternative<float>(signal->data)) {
			int data = (int)get<float>(signal->data);

			Engine::Instance().s_game->GetPlayer()->party->AddPartyMemeber(data);
		}
	}
	else if (signal->name == "StartCombat") {
		if (holds_alternative<string>(signal->data)) {
			vector<int> charsData;
			string characterIds = get<string>(signal->data);
			characterIds.erase(remove(characterIds.begin(), characterIds.end(), ' '));
			stringstream ss(characterIds);
			string temp;

			while (getline(ss, temp, ','))
			{
				charsData.emplace_back(stoi(temp));
			}

			Engine::Instance().s_game->SetCombat(charsData);
			Engine::Instance().s_game->SetState(GameScene::State::Combat );
		}
	}
	else if (signal->name == "CheckIfWonCombat") {
		dialogue->AddGameStateVariable("HasWonCombat",Engine::Instance().s_game->GetCombat()->HasWonLastCombat());
	}

	//Change dialogue box
}

