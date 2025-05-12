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


	void UpdateCanvas() override;

	void ChangeInventoryToTrack(Inventory* inventoryToTrack);
	void ChangePartyToTrack(Party* partyToTrack);

	void Reset();

private:

	struct UICharacterSelectorSlot {
		int characterId = -1;
		//CharacterDatabase::CharacterData* characterData = nullptr;

		UIButton* characterSelect = nullptr;

		UITextBox* characterName = nullptr;
	};

	struct UICharacterSlot {
		int characterId = -1;

		UIImage* characterOverlay = nullptr;
		UIImage* characterProfile = nullptr;

		UITextBox* characterName = nullptr;
	};

	struct UIItemSlots {
		int index = 0;
		Item* itemRef = nullptr;
		int amount;

		UIButton* itemSelect = nullptr;
		UIImage* itemImage = nullptr;
		UIImage* itemCoverImage = nullptr;
		UITextBox* itemAmount = nullptr;
	};

	void CreateCharacterSelectorSlots();
	void UpdateCharacterSelectorSlots();

	void CreateItemSlots();
	void UpdateItemSlots();

	void CreateCharacterSlot();
	void UpdateCharacterSlot(int charId);

	void CreateExtras();

	void OnItemSelected(int index);
	void OnMouseOverItem(int index);

private:
	Inventory* inventory = nullptr;
	Party* party = nullptr;

	UIImage* container_image = nullptr;

	vector<UICharacterSelectorSlot> selectorSlots;

	vector<UIItemSlots> itemSlots;

	UIImage* itemHoldingImage = nullptr;
	UIItemSlots* currentHoldedSlot = nullptr;
	UIItemSlots* mouseOverSlot = nullptr;
	

	UICharacterSlot characterSlot;
};
