#pragma once
#include "UICanvas.h"
#include <unordered_map>

class SettingsCG;

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

	unordered_map<string, UICanvas*> submenus;
};