#pragma once
#include "UICanvas.h"
#include <vector>

using namespace std;

class UIElement;
class UIImage;
class UIButton;
class UITextBox;
class UISlider;
class UIToggle;

class SaveLoadCG : public UICanvas {
public:
	SaveLoadCG(int _renderLayer);
	~SaveLoadCG() override = default;

private:
	
};
