#pragma once
#include "UICanvas.h"
#include <unordered_map>

class SettingsCG;
class InventoryCG;
class PartyCG;

class PauseMenuCG : public UICanvas {
public:
	PauseMenuCG();
	~PauseMenuCG();

	void Init();
	void CloseAllSubmenus();
	void OpenSubmenu(string menuName);

	void UpdateCanvas();
public:

private:

private:
	SettingsCG* settings;
	InventoryCG* inventory;
	PartyCG* party;

	unordered_map<string, UICanvas*> submenus;
	UICanvas* currentSubmenu = nullptr;

	float audio_boost = 0.2f;
};