#pragma once
#include "UICanvas.h"
#include <vector>

using namespace std;

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
	void UpdatePartySlots();

	void ChangePartyToTrack(Party* partyToTrack);

	void Reset();

	
public:

private:
	struct UICharacterSlot {
		int characterId = -1;

		UIButton* characterSelect = nullptr;
		UIImage* characterBackground = nullptr;

		UITextBox* charcterName = nullptr;
		UITextBox* charcterLevel = nullptr;


		UIImage* characterProfile = nullptr;

		UIToggle* addRemoveToggle = nullptr;
		UIToggle* switchToggle = nullptr;



		UIImage* hpBar = nullptr;
		UITextBox* hpValue = nullptr;
		int hpBarMaxWidth = 90;

		UIImage* energyBar = nullptr;
		UITextBox* energyValue = nullptr;
		int energyBarMaxWidth = 90;
	};

	void SwitchCharacter(int slot);
	void CreatePartySlots();
private:
	Party* party = nullptr;

	UIImage* container_image = nullptr;
	vector<UICharacterSlot> slots;

	bool switching = false;
	int switchingId = -1;

};
