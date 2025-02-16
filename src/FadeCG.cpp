#include "FadeCG.h"
#include "ModuleTime.h"

#include "UIImage.h"

#include <algorithm>

FadeCG::FadeCG(int r, int g, int b, Vector2Int _position, Vector2Int _size, Vector2 _pivot, float _scale, SDL_Texture* texture)
{
	fadeImage = new UIImage(*texture, _position, _size, _pivot, false, { 0,0,0,0 });
	fadeImage->SetLocalScale(_scale);
	SetColor(r, g, b);
	SetOpacity(0);

	AddElementToCanvas(fadeImage);
}

void FadeCG::UpdateCanvas()
{
	if (fading) {
		timer.Step(ModuleTime::deltaTime);

		//// Fade Effect

		float currentTime = timer.ReadSec();
		currentOpacity = (int)(startOpacity + (targetOpacity - startOpacity) * currentTime/timeToDoFade);
		SetOpacity(currentOpacity);
		
		////

		

		if (currentTime > timeToDoFade)
			fading = false;
	}
	UICanvas::UpdateCanvas();
}

void FadeCG::FadeIn(float fadeTime)
{
	timeToDoFade = fadeTime;
	fading = true;
	timer.Start();
	targetOpacity = 255;
	startOpacity = 0;
}

void FadeCG::FadeOut(float fadeTime)
{
	timeToDoFade = fadeTime;
	fading = true;
	timer.Start();
	targetOpacity = 0;
	startOpacity = 255;
}

void FadeCG::SetOpacity(int opacity)
{
	fadeImage->GetColor().a = opacity;
}

void FadeCG::SetColor(int r, int g, int b)
{
	fadeImage->GetColor().r = r;
	fadeImage->GetColor().g = g;
	fadeImage->GetColor().b = b;
}

bool FadeCG::IsFading()
{
	return fading;
}
