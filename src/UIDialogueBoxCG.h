#pragma once
#include "UICanvas.h"

class DialogueSystem;
class UITextBox;
class UIButton;

struct SDL_Rect;

class UIDialogueBoxCG : public UICanvas {
public:
	UIDialogueBoxCG();
	~UIDialogueBoxCG() override;

	void UpdateCanvas();
public:
	DialogueSystem* dialogue;
private:

private:
	float amistad = 0;
	UITextBox* contentTextBox;
	UITextBox* characterNameTextBox;

	///// Btn
	vector<UIButton*> btns;
};
