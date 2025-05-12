#pragma once
#include "UICanvas.h"
#include "StepTimer.h"
#include "Entity.h"
#include "Vector2.h"
#include <vector>

using namespace std;

class UIImage;
class UITextBox;

struct Layer {
	vector<UIImage*> images;
	Vector2 speed = {0,0};
	Vector2 offset = { 0,0 };
};

class ParallaxCG : public UICanvas {
public:
	ParallaxCG();
	~ParallaxCG() override = default;

	void UpdateCanvas();
	void UpdatePosition(Vector2 newPos);
	void AddLayer(SDL_Texture* texture, float speed);
	void AddLayer(SDL_Texture* texture, Vector2 speed);

private:

private:

	std::vector<Layer> layers;

	Vector2 previousCamPos = { 0,0 };
	Vector2 deltaMovement;
	
};
