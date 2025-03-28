#pragma once
#include "UICanvas.h"
#include "StepTimer.h"
#include <string>

class DialogueSystem;
class UITextBox;
class UIButton;
class UIImage;

struct Signal;

class UIDialogueBoxCG : public UICanvas {
public:
	UIDialogueBoxCG(DialogueSystem* _dialogueSystem);
	~UIDialogueBoxCG() override;

	void UpdateCanvas();

public:
	DialogueSystem* dialogue;
	
private:
	void CreateChoiceButton(string text, int index, float verticalSpacing);
	void SignalReader(Signal* signal);
	void ChangeDialogueNode();
	void EndDialogue();

	void NextDialogue();

	void SetVariablesOnStart();
private:

	float amistad = 0;
	UITextBox* contentTextBox = nullptr;
	UITextBox* characterNameTextBox = nullptr;
	UIImage* dialogueChoiceBox = nullptr;
	UIImage* dialogueTextBox = nullptr;

	UIImage* characterPortraitImage = nullptr;


	////Config
	bool typewriterMode = true;
	float typewriterSpeed = 0.05f;
	string typewriterText = "";
	StepTimer typewriterTimer;

	///// Btn
	vector<UIButton*> btns;
	int rodrigoState = 0;
	int guardState = 0;
};
