#pragma once
#include "UICanvas.h"

class DialogueSystem;
class UITextBox;

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
};
