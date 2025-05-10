#pragma once
#include "UICanvas.h"
#include "Vector2.h"
#include "Vector2Int.h"
#include <vector>

#include <SDL2/SDL_pixels.h>

using namespace std;

class UIImage;
class UIAnimatedImage;
class FadeCG;

class ScreenEffectsCG : public UICanvas {
public:
	ScreenEffectsCG(int _renderLayer);
	~ScreenEffectsCG();

	void UpdateCanvas();

	void StartRain();
	void StopRain();
	void SwitchRain();

	void RecalculateAmbientFadeColors();
	void ShowAmbient();
	void HideAmbient();

private:
	void CreateVignette();


	void CreateRainEffect();

	void CreateFirefliesEffect();

	void CreateAmbientFade();
	void UpdateAmbient(int _colorIndex= -1);
	int GetAmbientColorIndex();
	int GetAmbientColorIndex(int time);
private:

	UIImage* vignette = nullptr;


	int rainSoundChannel = -1;
	UIAnimatedImage* rainEffect = nullptr;
	FadeCG* rainFade = nullptr;


	FadeCG* ambientFade = nullptr;
	FadeCG* ambientFade_support = nullptr;

	int lastInterval = -1;
	float intervalTime = 24;
	vector<SDL_Color> ambientFadeColors = { {5, 4, 82, 255},{5, 4, 82, 255}, {255, 158, 60, 130},{5, 4, 82, 20},  {251, 84, 84, 0}, {251, 84, 84, 100} , {5, 4, 82, 150} , {5, 4, 82, 255}};
	vector<SDL_Color> ambientFadeSupportColors = { {35, 10, 158, 80}, {35, 10, 158, 80}, {35, 10, 158, 20}, {255,180,74,17}, {255,180,74,17}, {35, 10, 158, 20} , {35, 10, 158, 70} , {35, 10, 158, 80} };
};