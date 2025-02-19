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
private:
	float amistad = 0;
	UITextBox* contentTextBox;
	UITextBox* characterNameTextBox;
	UIImage* dialogueChoiceBox;
	UIImage* dialogueTextBox;

	///// Btn
	vector<UIButton*> btns;
};
