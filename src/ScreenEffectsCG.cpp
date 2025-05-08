#include "ScreenEffectsCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"
#include "GameScene.h"

#include "UIImage.h"
#include "UIAnimatedImage.h"
#include "FadeCG.h"

#include "Globals.h"

ScreenEffectsCG::ScreenEffectsCG(int _renderLayer)
{
	renderLayer = _renderLayer;
	CreateAmbientFade();
	CreateVignette();
	CreateRainEffect();
	CreateFirefliesEffect();

	StopRain();
}

ScreenEffectsCG::~ScreenEffectsCG()
{
	if (rainSoundChannel != -1)
		Engine::Instance().m_audio->StopSFX(rainSoundChannel);
	delete ambientFade;
	delete ambientFade_support;
}

void ScreenEffectsCG::UpdateCanvas()
{
	UpdateAmbient();

	ambientFade_support->UpdateCanvas();
	ambientFade->UpdateCanvas();

	UICanvas::UpdateCanvas();
}

void ScreenEffectsCG::StartRain()
{
	/// Turn On Visuals
	rainEffect->localVisible = true;

	if(rainSoundChannel!=-1)
		return;
	rainSoundChannel = Engine::Instance().m_audio->PlaySFX(Engine::Instance().m_assetsDB->GetAudio("rain_sfx"), -1);

}

void ScreenEffectsCG::StopRain()
{
	/// Turn Off Visuals
	rainEffect->localVisible = false;
	if (rainSoundChannel == -1)
		return;
	Engine::Instance().m_audio->StopSFX(rainSoundChannel);
	rainSoundChannel = -1;
	
}

void ScreenEffectsCG::SwitchRain()
{
	if (rainEffect->localVisible)
		StopRain();
	else
		StartRain();
}

void ScreenEffectsCG::CreateVignette()
{
	SDL_Texture* vignette_texture = Engine::Instance().m_assetsDB->GetTexture("vignette");
	Vector2Int texture_size = Engine::Instance().m_assetsDB->GetTextureSize(*vignette_texture);
	vignette = new UIImage(*vignette_texture, { 0,0 }, texture_size, { 0,0 }, false, { 0,0,0,0 }, { 0,0,0,150 });

	AddElementToCanvas(vignette);
}

void ScreenEffectsCG::CreateRainEffect()
{
	rainEffect = new UIAnimatedImage({ 0,0 }, { LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT });
	rainEffect->GetJsonAnimator()->AddJsonAnimationClip("Assets/Textures/Animations/rain_animation.json",0.04f);
	AddElementToCanvas(rainEffect);
}

void ScreenEffectsCG::CreateFirefliesEffect()
{

}


void ScreenEffectsCG::CreateAmbientFade()
{
	SDL_Texture* topFade_texture = Engine::Instance().m_assetsDB->GetTexture("top_fade");
	intervalTime = 24 / (int)ambientFadeColors.size();

	int colorIndex = GetAmbientColorIndex();
	colorIndex = abs(colorIndex - ((int)ambientFadeColors.size() - 1));
	lastInterval = colorIndex;
	SDL_Color color = ambientFadeColors[colorIndex];
	SDL_Color supportColor = ambientFadeSupportColors[colorIndex];

	ambientFade_support = new FadeCG(supportColor.r, supportColor.g, supportColor.b, supportColor.a, nullptr, { 0,0 });
	ambientFade_support->renderLayer = renderLayer;

	ambientFade = new FadeCG(color.r, color.g, color.b, color.a, topFade_texture, { 0,0 });
	ambientFade->renderLayer = renderLayer;
}

void ScreenEffectsCG::UpdateAmbient()
{

	
	int colorIndex = GetAmbientColorIndex();

	if (colorIndex != lastInterval)
	{
		lastInterval = colorIndex;
		float timeToDoFade = (intervalTime * 3600) / Engine::Instance().s_game->GetTimeScale();

		SDL_Color color = ambientFadeColors[colorIndex];
		SDL_Color supportColor = ambientFadeSupportColors[colorIndex];

		ambientFade_support->FadeTo(timeToDoFade, supportColor.r, supportColor.g, supportColor.b, supportColor.a);
		ambientFade->FadeTo(timeToDoFade, color.r, color.g, color.b, color.a);
	}
}

int ScreenEffectsCG::GetAmbientColorIndex()
{
	float totalTime = Engine::Instance().s_game->GetTime();
	int currentInterval = static_cast<int>(totalTime / (intervalTime * 3600));
	int colorIndex = currentInterval % ambientFadeColors.size();
	return colorIndex;
}
