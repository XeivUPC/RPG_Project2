#pragma once
#include "UICanvas.h"
#include "Vector2.h"
#include "Vector2Int.h"
#include <vector>

#include <SDL2/SDL_pixels.h>

using namespace std;

class UIImage;
class FadeCG;

class ScreenEffectsCG : public UICanvas {
public:
	ScreenEffectsCG(int _renderLayer);
	~ScreenEffectsCG();

	void UpdateCanvas();

private:
	void UpdateAmbient();
private:

	UIImage* vignette = nullptr;

	FadeCG* ambientFade = nullptr;
	FadeCG* ambientFade_support = nullptr;

	int currentAmbientColor = -1;
	vector<SDL_Color> ambientFadeColors = { {5, 4, 82, 255},{5, 4, 82, 0} };
	vector<SDL_Color> ambientFadeSupportColors = { {35, 10, 158, 80},{35, 10, 158, 0} };
};