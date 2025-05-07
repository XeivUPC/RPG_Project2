#include "ScreenEffectsCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "GameScene.h"

#include "UIImage.h"
#include "FadeCG.h"

ScreenEffectsCG::ScreenEffectsCG(int _renderLayer)
{
	SDL_Texture* topFade_texture = Engine::Instance().m_assetsDB->GetTexture("top_fade");
	SDL_Texture* vignette_texture = Engine::Instance().m_assetsDB->GetTexture("vignette");

	renderLayer = _renderLayer;

	Vector2Int texture_size = Engine::Instance().m_assetsDB->GetTextureSize(*vignette_texture);
	vignette = new UIImage(*vignette_texture, { 0,0 }, texture_size, { 0,0 }, false, { 0,0,0,0 }, {0,0,0,150});

	ambientFade_support = new FadeCG(35, 10, 158, 80, nullptr, { 0,0 });
	ambientFade_support->renderLayer = _renderLayer;

	ambientFade = new FadeCG(5, 4, 82, 255, topFade_texture, { 0,0 });
	ambientFade->renderLayer = _renderLayer;

	AddElementToCanvas(vignette);
}

ScreenEffectsCG::~ScreenEffectsCG()
{
	delete ambientFade;
	delete ambientFade_support;
}

void ScreenEffectsCG::UpdateCanvas()
{
	UpdateAmbient();

	ambientFade_support->UpdateCanvas();
	ambientFade->UpdateCanvas();
}

void ScreenEffectsCG::UpdateAmbient()
{

	int time = static_cast<int>(Engine::Instance().s_game->GetTime() / 3600) % 24;

	int currentIndex = currentAmbientColor;

	if (time >= 12 * (currentAmbientColor+1))
		currentIndex++;

	if (currentAmbientColor == currentIndex)
		return;

	if (currentIndex >= ambientFadeColors.size())
		currentIndex = 0;

	currentAmbientColor = currentIndex;

	float timeToDoFade = 86400 / 2 / Engine::Instance().s_game->GetTimeScale();

	SDL_Color color = ambientFadeColors[currentIndex];
	SDL_Color supportColor= ambientFadeSupportColors[currentIndex];

	ambientFade_support->FadeTo(timeToDoFade, supportColor.r, supportColor.g, supportColor.b, supportColor.a);
	ambientFade->FadeTo(timeToDoFade, color.r, color.g, color.b, color.a);
}
