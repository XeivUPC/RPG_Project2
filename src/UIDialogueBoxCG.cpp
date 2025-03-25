#include "UIDialogueBoxCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleTime.h"
#include "TextureAtlas.h"
#include "DialogueSystem.h"
#include "DrawingTools.h"

#include "UIImage.h"
#include "UIButton.h"
#include "UITextBox.h"

#include "Globals.h"

#include <sstream>

UIDialogueBoxCG::UIDialogueBoxCG()
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

	dialogue = new DialogueSystem();

	dialogue->onSignalCall.Subscribe([this](Signal* signal) {SignalReader(signal); });
	dialogue->onDialogNodeChange.Subscribe([this]() {ChangeDialogueNode(); });
	dialogue->onDialogEnd.Subscribe([this]() {EndDialogue(); });

	dialogue->AddGameStateVariable("friendship", amistad);
	dialogue->AddGameStateVariable("RodrigoState", (float)rodrigoState);

	dialogue->LoadDialogueWorkspace("Assets/Dialogues/Workspaces/RPG_Game.json");
	dialogue->LoadDialogueFromJSON("Assets/Dialogues/test2.json");
	amistad = 10;
	dialogue->StartDialogue();

}

UIDialogueBoxCG::~UIDialogueBoxCG()
{
	delete dialogue;
	UICanvas::~UICanvas();
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

void UIDialogueBoxCG::SetDialogue(string path)
{
	dialogue->LoadDialogueFromJSON(path);
	dialogue->StartDialogue();
}

void UIDialogueBoxCG::CreateChoiceButton(string text, int index, float verticalSpacing)
{
	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("alagard");

	UIButton* btn = new UIButton({ 388 ,(int)(123 + verticalSpacing + (22 + verticalSpacing) * index) }, { 235,22 }, { 0,0,0,0 }, { 0,0 }, {148,112,75,255});

	btn->onMouseClick.Subscribe([this, index]() {dialogue->ProcessInput(index); });

	UITextBox* textBox = new UITextBox(text, *font, 16, { 184,132,78,255 }, { 0,2 }, { 235,22 }, { 0,0}, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle, true);
	textBox->SetParent(btn);

	AddElementToCanvas(btn);

	btns.emplace_back(btn);
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
	else if (signal->name == "RodrigoStateUpdate") {
		if (holds_alternative<float>(signal->data)) {
			rodrigoState = (int)get<float>(signal->data);
			dialogue->AddGameStateVariable("RodrigoState", (float)rodrigoState);
		}
	}


	//Change dialogue box
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
