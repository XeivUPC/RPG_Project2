#pragma once
#include "UICanvas.h"

class Party;
class UIElement;
class UIImage;
class UIButton;
class UITextBox;
class UISlider;
class UIToggle;

class PartyCG : public UICanvas {
public:
	PartyCG();
	~PartyCG();

public:

private:
	struct UICharacterSlot {
		UIButton* characterSelect = nullptr;

		UITextBox* charcterName = nullptr;
		UITextBox* charcterLevel = nullptr;

		UIImage* characterProfile = nullptr;
		UIImage* characterColorFade = nullptr;

		UIImage* hpBar = nullptr;
		int hpBarMaxWidth = 0;

		UIImage* energyBar = nullptr;
		int energyBarMaxWidth = 0;
	};
private:
	Party* party = nullptr;
};
