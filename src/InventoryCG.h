#pragma once
#include "UICanvas.h"
#include <vector>

using namespace std;

class Inventory;
class Party;
class Item;

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

		UITextBox* characterName = nullptr;
	};

	struct UIItemSlots {
		Item* itemRef = nullptr;
		int amount;

		UIButton* itemSelect = nullptr;
	};

	void CreateCharacterSelectorSlots();
	void UpdateCharacterSelectorSlots();

	void CreateItemSlots();
	void UpdateItemSlots();

private:
	Inventory* inventory = nullptr;
	Party* party = nullptr;

	UIImage* container_image = nullptr;

	vector<UICharacterSelectorSlot> selectorSlots;

	vector<UIItemSlots> itemSlots;
};
