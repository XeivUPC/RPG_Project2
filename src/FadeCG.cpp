#include "FadeCG.h"
#include "ModuleTime.h"

#include "UIImage.h"

#include <algorithm>

FadeCG::FadeCG(int r, int g, int b, int a, SDL_Texture* texture, Vector2Int _position, Vector2Int _size, Vector2 _pivot, float _scale)
{
	fadeImage = new UIImage(*texture, _position, _size, _pivot, false, { 0,0,0,0 });
	fadeImage->SetLocalScale(_scale);
	SetColor(r, g, b, a);

	AddElementToCanvas(fadeImage);
}

void FadeCG::UpdateCanvas()
{
	if (fading) {
		timer.Step(ModuleTime::deltaTime);

		//// Fade Effect

		float currentTime = timer.ReadSec();

		float percentage = currentTime / timeToDoFade;
		int r = (int)(startingColor.r + (targetColor.r - startingColor.r) * percentage);
		int g = (int)(startingColor.g + (targetColor.g - startingColor.g) * percentage);
		int b = (int)(startingColor.b + (targetColor.b - startingColor.b) * percentage);
		int a = (int)(startingColor.a + (targetColor.a - startingColor.a) * percentage);

		SetColor(r,g,b,a);
		
		////

		

		if (currentTime > timeToDoFade) {
			SetColor(targetColor.r, targetColor.g, targetColor.b, targetColor.a);
			fading = false;
			onFadeEnd.Trigger();
			onFadeEnd.UnsubscribeAll();
		}
	}
	UICanvas::UpdateCanvas();
}

void FadeCG::FadeTo(float fadeTime, int r, int g, int b, int a)
{
	timeToDoFade = fadeTime;
	fading = true;
	timer.Start();

	SDL_Color& current = fadeImage->GetColor();
	startingColor = { current.r,current.g,current.b,current.a };
	targetColor = { r,g,b,a };
}

void FadeCG::FadeTo(float fadeTime, int a)
{
	timeToDoFade = fadeTime;
	fading = true;
	timer.Start();

	SDL_Color& current = fadeImage->GetColor();
	startingColor = { current.r,current.g,current.b,current.a };
	targetColor = { current.r,current.g,current.b,a };
}

void FadeCG::FadeTo(float fadeTime, int r, int g, int b)
{
	timeToDoFade = fadeTime;
	fading = true;
	timer.Start();

	SDL_Color& current = fadeImage->GetColor();
	startingColor = { current.r,current.g,current.b,current.a };
	targetColor = { r,g,b,current.a };
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

void FadeCG::SetColor(int r, int g, int b, int a)
{
	SetColor(r,g,b);
	SetOpacity(a);
}

void FadeCG::SetFadeTimer(int time)
{
	timer = StepTimer(time);
}

bool FadeCG::IsFading()
{
	return fading;
}
