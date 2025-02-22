#pragma once
#include "UICanvas.h"

class TitleScene;

class TitleMenuCG : public UICanvas {
public:
	TitleMenuCG(UICanvas& _settingsReference);
	~TitleMenuCG() override = default;

public:

private:

private:
	UICanvas* settingsReference = nullptr;
};