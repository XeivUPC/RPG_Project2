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
	InventoryCG(int _renderLayer);
	~InventoryCG();


	void UpdateCanvas() override;

	void LoadUseSlot();
	void SaveUseSlot();
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
		Inventory* inventory = nullptr;
		int index = 0;
		Item* itemRef = nullptr;
		int amount;

		UIButton* itemSelect = nullptr;
		UIImage* itemImage = nullptr;
		UIImage* itemCoverImage = nullptr;
		UITextBox* itemAmount = nullptr;
	};

	struct UICharacterSlot {
		string characterId = "";

		UIImage* characterOverlay = nullptr;
		UIImage* characterProfile = nullptr;
		UITextBox* characterName = nullptr;


		UIItemSlots helmetSlot;
		UIItemSlots chestplateSlot;

		UIItemSlots weaponSlot;
		UIItemSlots accesorieSlot;
	};

	struct UIUseSlot {
		UIButton* useButton = nullptr;
		Inventory* useInventory = nullptr;
		UIItemSlots useSlot;
	};;


	void CreateCharacterSelectorSlots();
	void UpdateCharacterSelectorSlots();

	void GoToMemebersOffset(int _membersOffset);

	UIItemSlots CreateItemSlot(Inventory* inventory, int index, Item* itemRef, int amount, int textureType = 0);
	void UpdateItemSlot(UIItemSlots* slot, Inventory* inventory, Item* itemRef, int amount);
	void UpdateItemSlot(UIItemSlots* slot, Inventory* inventory, int index);
	void CreateItemSlots();
	void UpdateItemSlots();

	void CreateCharacterSlot();
	void UpdateCharacterSlot(string charId);

	void CreateUseSlot();
	void ConsumeItem();

	void CreateExtras();

	void OnItemSelected(UIItemSlots* slot);
	void OnMouseOverItem(UIItemSlots* slot);

private:
	Inventory* playerInventory = nullptr;
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
	UIUseSlot useSlot;
};
