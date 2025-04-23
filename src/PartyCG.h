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

	void UpdateMemberSlots();
	void GoToMemeberPage(int page);

	void ChangePartyToTrack(Party* partyToTrack);

	void Reset();

	
public:

private:

	struct UIPartyCharacterSlot {
		int characterId = -1;

		UIButton* characterSelect = nullptr;
		UIButton* chracterOverlay = nullptr;

		UIImage* characterProfile = nullptr;
	};

	struct UIMemberCharacterSlot {
		int characterId = -1;

		UIButton* characterSelect = nullptr;
		UIImage* characterBackground = nullptr;

		UITextBox* charcterName = nullptr;
		UITextBox* charcterLevel = nullptr;


		UIImage* characterProfile = nullptr;

		UIToggle* addRemoveToggle = nullptr;

		UIImage* hpBar = nullptr;
		UITextBox* hpValue = nullptr;
		int hpBarMaxWidth = 90;

		UIImage* energyBar = nullptr;
		UITextBox* energyValue = nullptr;
		int energyBarMaxWidth = 90;
	};

	void RemovePartyCharacter(int id);
	void AddPartyCharacter(int id);

	void SwitchCharacter(int slot);
	void CreatePartySlots();

	void CreateMemeberSlots();
private:
	Party* party = nullptr;

	UIImage* container_image = nullptr;

	UIButton* prev_pageBtn = nullptr;
	UIButton* next_pageBtn = nullptr;

	UITextBox* pageData = nullptr;

	vector<UIPartyCharacterSlot> partySlots;
	vector<UIMemberCharacterSlot> memberSlots;

	bool switching = false;
	int switchingId = -1;

	int membersPage = -1;
	int membersByPage = 4;

};
