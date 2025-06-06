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
	delete rainFade;
}

void ScreenEffectsCG::UpdateCanvas()
{
	UpdateAmbient();

	ambientFade_support->UpdateCanvas();
	ambientFade->UpdateCanvas();
	rainFade->UpdateCanvas();

	UICanvas::UpdateCanvas();
}

void ScreenEffectsCG::StartRain()
{
	/// Turn On Visuals
	rainEffect->localVisible = true;

	if(rainSoundChannel!=-1)
		return;
	rainSoundChannel = Engine::Instance().m_audio->PlaySFXWithFade(Engine::Instance().m_assetsDB->GetAudio("rain_sfx"), rainSoundChannel, -1,1000);
	rainFade->FadeTo(1, 50);

}

void ScreenEffectsCG::StopRain()
{
	/// Turn Off Visuals
	rainEffect->localVisible = false;
	if (rainSoundChannel == -1)
		return;
	Engine::Instance().m_audio->StopSFX(rainSoundChannel,1000);
	rainSoundChannel = -1;

	rainFade->FadeTo(1,0);
	
}

void ScreenEffectsCG::SwitchRain()
{
	if (rainEffect->localVisible)
		StopRain();
	else
		StartRain();
}


void ScreenEffectsCG::ShowAmbient()
{
	ambientFade_support->isVisible = true;
	ambientFade->isVisible = true;
}

void ScreenEffectsCG::HideAmbient()
{
	ambientFade_support->isVisible = false;
	ambientFade->isVisible = false;
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

	rainFade = new FadeCG(0, 0, 0, 0, nullptr, { 0,0 });
	rainFade->renderLayer = renderLayer;

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
	intervalTime = 24.f / (int)ambientFadeColors.size();

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

void ScreenEffectsCG::RecalculateAmbientFadeColors()
{

	int totalTime = Engine::Instance().s_game->GetTime();
	float intervalDuration = intervalTime * 3600; 
	int currentInterval = static_cast<int>(totalTime / intervalDuration);
	float intervalStartTime = currentInterval * intervalDuration;

	float timePassedInInterval = (totalTime - intervalStartTime) / Engine::Instance().s_game->GetTimeScale();

	lastInterval = -1;
	
	int colorIndex = (currentInterval - 1 + ambientFadeColors.size()) % (int)ambientFadeColors.size();
	SDL_Color color = ambientFadeColors[colorIndex];
	SDL_Color supportColor = ambientFadeSupportColors[colorIndex];

	ambientFade->SetColor(color.r, color.g, color.b, color.a);
	ambientFade_support->SetColor(supportColor.r, supportColor.g, supportColor.b, supportColor.a);
	UpdateAmbient();

	ambientFade->SetFadeTimer((int)timePassedInInterval);
	ambientFade_support->SetFadeTimer((int)timePassedInInterval);


}

void ScreenEffectsCG::UpdateAmbient(int _colorIndex)
{


	int colorIndex = _colorIndex;
	if(colorIndex==-1)
		colorIndex = GetAmbientColorIndex();

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
	int totalTime = Engine::Instance().s_game->GetTime();
	return GetAmbientColorIndex(totalTime);
}

int ScreenEffectsCG::GetAmbientColorIndex(int time)
{
	int totalTime = time;
	int currentInterval = static_cast<int>(totalTime / (intervalTime * 3600));
	int colorIndex = currentInterval % ambientFadeColors.size();
	return colorIndex;
}
