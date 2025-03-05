#pragma once
#include "UICanvas.h"
#include <string>

class DialogueSystem;
class UITextBox;
class UIButton;
class UIImage;

struct SDL_Rect;
struct Signal;

class UIDialogueBoxCG : public UICanvas {
public:
	UIDialogueBoxCG();
	~UIDialogueBoxCG() override;

	void UpdateCanvas();
public:
	DialogueSystem* dialogue;
private:
	void CreateChoiceButton(string text, int index, float verticalSpacing);
	void SignalReader(Signal* signal);
	void ChangePortrait();
	void EndDialogue();
private:
	float amistad = 0;
	UITextBox* contentTextBox = nullptr;
	UITextBox* characterNameTextBox = nullptr;
	UIImage* dialogueChoiceBox = nullptr;
	UIImage* dialogueTextBox = nullptr;

	UIImage* characterPortraitImage = nullptr;

	///// Btn
	vector<UIButton*> btns;
	int rodrigoState = 0;
};
