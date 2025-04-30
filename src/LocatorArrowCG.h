#pragma once
#include "UICanvas.h"
#include "Vector2.h"
#include <vector>

using namespace std;

class UIImage;

class LocatorArrowCG : public UICanvas {
public:
	LocatorArrowCG();
	~LocatorArrowCG() override = default;

	void UpdateCanvas();

	void SetLocation(Vector2 locationTarget);
	void GetLocation(Vector2 locationTarget);

public:

private:

	
private:
	UIImage* coverlay_image = nullptr;
	UIImage* arrow_image = nullptr;
};
