#pragma once
#include "UICanvas.h"
#include "StepTimer.h"
#include "Entity.h"
#include "Vector2.h"
#include <vector>

using namespace std;

class UIImage;
class UITextBox;
class MissionsCG;

class GameplayCG : public UICanvas {
public:
	GameplayCG(int _renderLayer);
	~GameplayCG();

	void UpdateCanvas();
	
	void SetUser(Entity* _user);
	Entity* GetUser() const;

	void SetLocation(Vector2 locationTarget);
	void RemoveLocation();
	Vector2 GetLocation() const;

private:
	void UpdateCompass();
	void UpdateClock();

private:
	MissionsCG* missionCanvas = nullptr;


	Entity* user = nullptr;
	Vector2 targetLocation = {0,0};

	bool compassHasLocation = false;

	UIImage* overlay_image = nullptr;
	UIImage* arrow_image = nullptr;
	UITextBox* clock_text = nullptr;

	Vector2Int border = { 20, 8 };

	//Compass has location
	SDL_Rect overlay_rect1 = { 0,0,67,120 };
	Vector2Int clock_text_pos1 = { 1,110 - 60 };

	//Compass does not have location
	SDL_Rect overlay_rect2 = { 67,0,67,120 };
	Vector2Int clock_text_pos2 = { 1, -48 };
};
