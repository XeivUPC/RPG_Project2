#pragma once
#include "UICanvas.h"
#include <vector>

using namespace std;

class Inventory;
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

	void ChangeInventoryToTrack(Inventory* partyToTrack);

	void Reset();

private:
	Inventory* inventory = nullptr;
};
