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
	UIImage* image = new UIImage(*tex, { 0,0 }, textureSize);
	AddElementToCanvas(image);

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
	dialogue->LoadDialogueFromJSON("Assets/Dialogues/test.json");
	amistad = 10;
	dialogue->AddGameStateVariable("friendship", amistad);
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
		const DialogueNode& node = dialogue->GetCurrentDialogue();
		contentTextBox->text = node.text;
		if (node.choices.size() > 1)
		{
			btns[0]->isEnabled = false;
		}else 
			btns[0]->isEnabled = true;
		for (int i = 1; i < node.choices.size()+1; i++)
		{
			if (((int)btns.size()) - 1 < node.choices.size()) {
				SDL_Texture* nullTexture = nullptr;
				UIButton* btn = new UIButton(*nullTexture, { 776 ,247 + 50 * (i - 1)}, { 470,45 }, { 0,0,0,0 }, { 0,0 });
				btn->onMouseClick.emplace_back([this,i]() {dialogue->ProcessInput(i - 1); });
				btn->debug = true;
				AddElementToCanvas(btn);

				btns.emplace_back(btn);
			}
			

		}
		
		characterNameTextBox->text = node.characterName;
	}
	else {
		contentTextBox->text = "";
		characterNameTextBox->text = "";

	}

	UICanvas::UpdateCanvas();
}
