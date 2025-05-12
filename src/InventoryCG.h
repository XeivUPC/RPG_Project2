#pragma once
#include "UICanvas.h"
#include <vector>

using namespace std;

class Inventory;
class Party;
class UIElement;
class UIImage;
class UIButton;
class UITextBox;
class UISlider;
class UIToggle;

class InventoryCG : public UICanvas {
public:
	InventoryCG();
	~InventoryCG();

	void ChangeInventoryToTrack(Inventory* inventoryToTrack);
	void ChangePartyToTrack(Party* partyToTrack);

	void Reset();

private:

	struct UICharacterSelectorSlot {
		int characterId = -1;

		UIButton* characterSelect = nullptr;
		UIImage* characterOverlay = nullptr;

		UITextBox* characterName = nullptr;
	};

	void CreateCharacterSelectorSlots();
	void UpdateCharacterSelectorSlots();

private:
	Inventory* inventory = nullptr;
	Party* party = nullptr;

	UIImage* container_image = nullptr;

	vector<UICharacterSelectorSlot> selectorSlots;
};
