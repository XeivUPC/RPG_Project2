#pragma once
#include "UICanvas.h"
#include "StepTimer.h"
#include "Entity.h"
#include "Vector2.h"
#include <vector>

using namespace std;

class UIImage;
class UITextBox;

class GameplayCG : public UICanvas {
public:
	GameplayCG();
	~GameplayCG() override = default;

	void UpdateCanvas();
	
	void SetUser(Entity* _user);
	Entity* GetUser() const;

	void SetLocation(Vector2 locationTarget);
	Vector2 GetLocation() const;

private:
	void UpdateCompass();
	void UpdateClock();

private:
	Entity* user = nullptr;
	Vector2 targetLocation = {0,0};

	bool locationInsideScreen = false;

	UIImage* overlay_image = nullptr;
	UIImage* arrow_image = nullptr;
	UITextBox* clock_text = nullptr;

	Vector2Int border = { 20, 8 };
};
