#pragma once
#include "UICanvas.h"

class SettingsCG;

class PauseMenuCG : public UICanvas {
public:
	PauseMenuCG();
	~PauseMenuCG();

	void UpdateCanvas();
public:

private:

private:
	SettingsCG* settings;
};