#pragma once
#include "UICanvas.h"
#include "Entity.h"
#include "Vector2.h"
#include <vector>

using namespace std;

class UIImage;

class LocatorArrowCG : public UICanvas {
public:
	LocatorArrowCG();
	~LocatorArrowCG() override = default;

	void UpdateCanvas();
	void UpdateInsideScreen();
	void UpdateOutsideScreen();
	
	void SetUser(Entity* _user);
	Entity* GetUser() const;

	void SetLocation(Vector2 locationTarget);
	Vector2 GetLocation() const;

public:

private:
	Entity* user = nullptr;
	Vector2 targetLocation = {0,0};

	bool locationInsideScreen = false;

	UIImage* overlay_image = nullptr;
	UIImage* arrow_image = nullptr;
};
