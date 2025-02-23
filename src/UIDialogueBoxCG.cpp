#include "UIDialogueBoxCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "DialogueSystem.h"
#include "DrawingTools.h"

#include "UIImage.h"
#include "UIButton.h"
#include "UITextBox.h"

UIDialogueBoxCG::UIDialogueBoxCG()
{
	SDL_Texture* tex = Engine::Instance().m_assetsDB->GetTexture("dialogue_box");
	SDL_Texture* tex2 = Engine::Instance().m_assetsDB->GetTexture("dialogue_answerBox");
	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("monogram");

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*tex);
	dialogueTextBox = new UIImage(*tex, { 0,0 }, textureSize);
	dialogueChoiceBox = new UIImage(*tex2, { 0,0 }, textureSize);
	AddElementToCanvas(dialogueTextBox);
	AddElementToCanvas(dialogueChoiceBox);

	contentTextBox = new UITextBox("", *font, 64, { 255,255,255,255 }, { 12,574 }, { 1223,137 }, {0,0}, UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment::Top, true);
	AddElementToCanvas(contentTextBox);

	characterNameTextBox = new UITextBox("", *font, 64, { 255,255,255,255 }, { 12, 511 }, { 481,47 }, { 0,0 }, UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment::Top, true);
	AddElementToCanvas(characterNameTextBox);

	UIButton* btn = new UIButton(*tex, { 0,559 }, { 1280,161 }, { 0,0,0,0 }, { 0,0 });
	btn->onMouseClick.emplace_back([this]() {dialogue->ProcessInput(0); });
	btn->SetLocalScale(1);
	AddElementToCanvas(btn);

	btns.emplace_back(btn);

	dialogue = new DialogueSystem();
	dialogue->AddGameStateVariable("friendship", amistad);
	dialogue->AddGameStateVariable("RodrigoState", (float)rodrigoState);

	dialogue->LoadDialogueFromJSON("Assets/Dialogues/test2.json");
	amistad = 10;
	dialogue->StartDialogue();

	dialogue->onSignalCall.emplace_back([this](Signal* signal) {SignalReader(signal); });
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
		contentTextBox->SetText(node.text);
		if (node.choices.size() > 1)
		{
			btns[0]->isEnabled = false;
			dialogueChoiceBox->GetColor().a = 255;

			if (node.choices.size() == ((int)btns.size()) - 1) {
				for (int i = 1; i < node.choices.size(); i++)
				{
					
				}
			}
			else {	
				while (btns.size() != 1) {
					RemoveElementFromCanvas(btns.back());
					delete btns.back();
					btns.pop_back();
				}
				for (int i = 1; i < node.choices.size() + 1; i++)
				{
					CreateChoiceButton(node.choices[i - 1].text, i - 1, 5);
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
		
		
		characterNameTextBox->SetText(node.characterName);
	}
	else {
		dialogueTextBox->GetColor().a = 0;
		contentTextBox->SetText("");
		characterNameTextBox->SetText("");


	}

	UICanvas::UpdateCanvas();
}

void UIDialogueBoxCG::CreateChoiceButton(string text, int index, float verticalSpacing)
{
	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("monogram");

	UIButton* btn = new UIButton({ 776 ,(int)(247 + verticalSpacing+ (45 + verticalSpacing) * index )}, { 470,45 }, { 0,0,0,0 }, { 0,0 });
	btn->onMouseClick.emplace_back([this, index]() {dialogue->ProcessInput(index); });
	btn->debug = true;

	UITextBox* textBox = new UITextBox(text, *font, 32, { 0,0,0,255 }, { 0,0 }, { 470,45 }, { 0,0}, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle, true);
	textBox->SetParent(btn);

	AddElementToCanvas(btn);
	AddElementToCanvas(textBox);

	btns.emplace_back(btn);
}

void UIDialogueBoxCG::SignalReader(Signal* signal)
{
	if (signal->name == "ChangeDialogue") {
		if (holds_alternative<string>(signal->data)) {
			dialogue->LoadDialogueFromJSON(get<string>(signal->data));
		}
	}
	else if (signal->name == "RodrigoStateUpdate") {
		if (holds_alternative<float>(signal->data)) {
			rodrigoState = (int)get<float>(signal->data);
			dialogue->AddGameStateVariable("RodrigoState", (float)rodrigoState);
		}
	}
}
