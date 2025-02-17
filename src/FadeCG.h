#pragma once
#include "UICanvas.h"
#include "StepTimer.h"
#include "Vector2.h"
#include "Vector2Int.h"

#include "Globals.h"

class UIImage;
struct SDL_Texture;


class FadeCG : public UICanvas {
public:
	FadeCG(int r, int g, int b, Vector2Int _position = { 0,0 }, Vector2Int _size = { LOGIC_SCREEN_WIDTH, LOGIC_SCREEN_HEIGHT }, Vector2 _pivot = {0,0}, float _scale = 1, SDL_Texture* texture = nullptr);
	~FadeCG() override = default;

	void UpdateCanvas();

	void FadeIn(float fadeTime);
	void FadeOut(float fadeTime);

	void SetOpacity(int opacity);
	void SetColor(int r, int g, int b);

	bool IsFading();
public:

private:
private:
	UIImage* fadeImage;
	bool fading=false;

	float timeToDoFade = 0;
	StepTimer timer;

	int currentOpacity=0;
	int targetOpacity = 255;
	int startOpacity = 0;
};