#pragma once
#include "UICanvas.h"
#include <unordered_map>

class SettingsCG;
class InventoryCG;
class SaveLoadCG;
class PartyCG;

class PauseMenuCG : public UICanvas {
public:
	PauseMenuCG(int _renderLayer);
	~PauseMenuCG();

	void Init();
	void CloseAllSubmenus();
	void OpenSubmenu(string menuName);

	void OpenInventory();
	void OpenParty();

	void UpdateCanvas();
public:

private:

private:
	SettingsCG* settings;
	InventoryCG* inventory;
	SaveLoadCG* saveLoad;
	PartyCG* party;

	unordered_map<string, UICanvas*> submenus;
	UICanvas* currentSubmenu = nullptr;

	float audio_boost = 0.2f;
};