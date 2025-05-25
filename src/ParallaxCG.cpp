#include "ParallaxCG.h"

#include "Engine.h"
#include "ModuleTime.h"
#include "ModuleAssetDatabase.h"
#include "GameScene.h"

#include "UIImage.h"
#include "UITextBox.h"
#include "Globals.h"

#include <iomanip>
#include <sstream>

ParallaxCG::ParallaxCG()
{

}

void ParallaxCG::UpdateCanvas()
{
	for (auto& layer : layers) {

		Vector2 movementValue;
		movementValue.x = deltaMovement.x * layer.speed.x;
		movementValue.y = deltaMovement.y * layer.speed.y;
		layer.offset += movementValue;

		if (fabs(layer.offset.x) > layer.images[0]->size.x) {
			layer.offset.x = 0.0f;
		}
		if (fabs(layer.offset.y) > layer.images[0]->size.y) {
			layer.offset.y = 0.0f;
		}


		int globalIndex = 0;
		for (int x = -1; x <= 1; ++x) {
			for (int y = -1; y <= 1; y++)
			{
				Vector2 position = { layer.offset.x + layer.images[globalIndex]->size.x * x, layer.offset.y + layer.images[globalIndex]->size.y * y };
				layer.images[globalIndex]->SetLocalPosition(position);
				globalIndex++;

			}
		}
	}

	UICanvas::UpdateCanvas();
}

void ParallaxCG::UpdatePosition(Vector2 newPos)
{
	deltaMovement = previousCamPos - newPos;
	previousCamPos = newPos;
}

void ParallaxCG::AddLayer(SDL_Texture* texture, float speed)
{
	AddLayer(texture, { speed, speed });
}

void ParallaxCG::AddLayer(SDL_Texture* texture, Vector2 speed)
{
	Vector2 size = Engine::Instance().m_assetsDB->GetTextureSize(*texture);
	vector<UIImage*> images;

	for (size_t i = 0; i < 9; i++)
	{
		UIImage* image = new UIImage(*texture, { 0,0 }, size, { 0,0 });
		images.emplace_back(image);
		AddElementToCanvas(image);
	}

	layers.emplace_back(Layer{ images, speed });
}
