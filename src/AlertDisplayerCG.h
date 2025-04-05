#pragma once
#include "UICanvas.h"
#include "StepTimer.h"
#include "Vector2.h"
#include "SystemEvent.h"
#include "Globals.h"

#include <string>

class UIImage;
class UITextBox;
struct SDL_Texture;

class AlertDisplayerCG : public UICanvas {
public:
	AlertDisplayerCG(float _hideTime,SDL_Texture* texture = nullptr, Vector2Int _position = { 0,0 }, Vector2Int _size = { LOGIC_SCREEN_WIDTH, LOGIC_SCREEN_HEIGHT }, Vector2 _pivot = { 0,0 }, float _scale = 1);
	~AlertDisplayerCG() override = default;

	void UpdateCanvas();

	void SetAlertData(string alertInfo);
	void SetAlertData(float _hideTime);
	void SetAlertData(float _hideTime, string alertInfo);
	void OpenAlert();
	void CloseAlert();

public:
	SystemEvent<> onAlertOpened;
	SystemEvent<> onAlertClosed;

private:


private:
	float hideTime=0;
	StepTimer timer;

	UIImage* alert=nullptr;
	UITextBox* text=nullptr;
};