#pragma once
#include "UICanvas.h"

struct Mix_Chunk;

class UITestingCG : public UICanvas {
public:
	UITestingCG();
	~UITestingCG() override = default;
public:

private:

private:
	Mix_Chunk* btn_enter;
};