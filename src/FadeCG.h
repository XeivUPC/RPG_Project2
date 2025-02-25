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
	FadeCG(int r, int g, int b, int a, SDL_Texture* texture = nullptr, Vector2Int _position = { 0,0 }, Vector2Int _size = { LOGIC_SCREEN_WIDTH, LOGIC_SCREEN_HEIGHT }, Vector2 _pivot = {0,0}, float _scale = 1);
	~FadeCG() override = default;

	void UpdateCanvas();

	void FadeTo(float fadeTime, int r, int g, int b, int a);
	void FadeTo(float fadeTime, int a);
	void FadeTo(float fadeTime, int r, int g, int b);

	void SetOpacity(int opacity);
	void SetColor(int r, int g, int b);
	void SetColor(int r, int g, int b, int a);



	bool IsFading();
public:

private:
	struct Color {
		int r;
		int g;
		int b;
		int a;
	};


private:
	UIImage* fadeImage;
	bool fading=false;

	float timeToDoFade = 0;
	StepTimer timer;


	Color startingColor;
	Color targetColor;
};