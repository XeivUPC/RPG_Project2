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
	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("alagard");

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*tex);
	dialogueTextBox = new UIImage(*tex, { 0,0 }, textureSize);
	dialogueChoiceBox = new UIImage(*tex2, { 0,0 }, textureSize);
	AddElementToCanvas(dialogueTextBox);
	AddElementToCanvas(dialogueChoiceBox);

	contentTextBox = new UITextBox("", *font, 32, { 184,132,78,255 }, { 33,582 }, { 1173,117 }, {0,0}, UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment::Top, true);
	AddElementToCanvas(contentTextBox);

	characterNameTextBox = new UITextBox("", *font, 32, { 184,132,78,255 }, { 12, 521 }, { 461,37 }, { 0,0 }, UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment::Middle, true);
	AddElementToCanvas(characterNameTextBox);

	UIButton* btn = new UIButton(*tex, { 0,559 }, { 1280,161 }, { 0,0,0,0 }, { 0,0 });
	btn->onMouseClick.emplace_back([this]() {dialogue->ProcessInput(0); });
	btn->SetLocalScale(1);
	AddElementToCanvas(btn);


	characterPortraitImage = new UIImage({ 0,511 }, { 64, 64 }, { 0,1 }, true, { 0,0,64,64 }, {255,255,255,255});
	characterPortraitImage->SetLocalScale(4);
	AddElementToCanvas(characterPortraitImage);



	btns.emplace_back(btn);

	dialogue = new DialogueSystem();

	dialogue->onSignalCall.emplace_back([this](Signal* signal) {SignalReader(signal); });
	dialogue->onDialogNodeChange.emplace_back([this]() {ChangePortrait(); });
	dialogue->onDialogEnd.emplace_back([this]() {EndDialogue(); });

	dialogue->AddGameStateVariable("friendship", amistad);
	dialogue->AddGameStateVariable("RodrigoState", (float)rodrigoState);

	dialogue->LoadDialogueWorkspace("Assets/Dialogues/Workspaces/RPG_Game.json");
	dialogue->LoadDialogueFromJSON("Assets/Dialogues/test3.json");
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
		
		
		characterNameTextBox->SetText(node.character.name);
	}

	UICanvas::UpdateCanvas();
}

void UIDialogueBoxCG::CreateChoiceButton(string text, int index, float verticalSpacing)
{
	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("alagard");

	UIButton* btn = new UIButton({ 776 ,(int)(247 + verticalSpacing + (45 + verticalSpacing) * index) }, { 470,45 }, { 0,0,0,0 }, { 0,0 }, {148,112,75,255});

	btn->onMouseClick.emplace_back([this, index]() {dialogue->ProcessInput(index); });

	UITextBox* textBox = new UITextBox(text, *font, 32, { 184,132,78,255 }, { 0,5 }, { 470,45 }, { 0,0}, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle, true);
	textBox->SetParent(btn);

	AddElementToCanvas(btn);
	AddElementToCanvas(textBox);

	btns.emplace_back(btn);
}

void UIDialogueBoxCG::SignalReader(Signal* signal)
{
	if (signal->name == "RodrigoStateUpdate") {
		if (holds_alternative<float>(signal->data)) {
			rodrigoState = (int)get<float>(signal->data);
			dialogue->AddGameStateVariable("RodrigoState", (float)rodrigoState);
		}
	}
}

void UIDialogueBoxCG::ChangePortrait()
{
	const DialogueNode& node = dialogue->GetCurrentDialogue();
	characterPortraitImage->SetColor({ 255, 255, 255, 0 });
	for (size_t i = 0; i < node.character.portraits.size(); i++)
	{
		if (node.character.portraits[i].id.find(node.portraitId) != std::string::npos)
		{
			SDL_Texture* tex = Engine::Instance().m_assetsDB->GetTexture(node.character.portraits[i].name);
			characterPortraitImage->SetSprite(*tex, true, {0,0,64,64});
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
