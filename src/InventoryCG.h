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
		string characterId = "character;test";
		//CharacterDatabase::CharacterDefinition* characterData = nullptr;

		UIButton* characterSelect = nullptr;

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

	struct UICharacterSlot {
		int characterId = -1;

		UIImage* characterOverlay = nullptr;
		UIImage* characterProfile = nullptr;
		UITextBox* characterName = nullptr;


		UIItemSlots* helmetSlot = nullptr;
		UIItemSlots* chestplateSlot = nullptr;
		UIItemSlots* leggingsSlot = nullptr;
		UIItemSlots* bootsSlot = nullptr;

		UIItemSlots* weaponSlot = nullptr;
		UIItemSlots* accesorieSlot = nullptr;
	};

	

	void CreateCharacterSelectorSlots();
	void UpdateCharacterSelectorSlots();

	void GoToMemebersOffset(int _membersOffset);

	void CreateItemSlots();
	void UpdateItemSlots();

	void CreateCharacterSlot();
	void UpdateCharacterSlot(string charId);

	void CreateExtras();

	void OnItemSelected(int index);
	void OnMouseOverItem(int index);

private:
	Inventory* inventory = nullptr;
	Party* party = nullptr;

	UIImage* container_image = nullptr;

	vector<UICharacterSelectorSlot> selectorSlots;
	UIButton* prev_pageBtn = nullptr;
	UIButton* next_pageBtn = nullptr;
	int membersOffset = -1;
	int membersByPage = 6;



	vector<UIItemSlots> itemSlots;

	UIImage* itemHoldingImage = nullptr;
	UIItemSlots* currentHoldedSlot = nullptr;
	UIItemSlots* mouseOverSlot = nullptr;
	

	UICharacterSlot characterSlot;
};
