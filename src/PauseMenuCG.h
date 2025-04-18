#pragma once
#include "UICanvas.h"
#include <unordered_map>

class SettingsCG;
class PartyCG;

class PauseMenuCG : public UICanvas {
public:
	PauseMenuCG();
	~PauseMenuCG();

	void CloseAllSubmenus();
	void OpenSubmenu(string menuName);

	void UpdateCanvas();
public:

private:

private:
	SettingsCG* settings;
	PartyCG* party;

	unordered_map<string, UICanvas*> submenus;
	UICanvas* currentSubmenu = nullptr;
};