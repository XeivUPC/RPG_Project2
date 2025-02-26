#include "ModuleCursor.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "DrawingTools.h"

ModuleCursor::ModuleCursor(bool start_active) : Module(start_active)
{

}

ModuleCursor::~ModuleCursor()
{
}

bool ModuleCursor::Init()
{
	renderLayer = 10000;
	return true;
}

bool ModuleCursor::Start()
{
	Engine::Instance().m_render->AddToRenderQueue(*this);
	SetCursor(activeCursor.texture, activeCursor.rect);
	return true;
}

bool ModuleCursor::CleanUp()
{
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);
	return true;
}

void ModuleCursor::Render()
{
	if (hiddenCustom)
		return;
	Vector2Int mousePos = Engine::Instance().m_input->GetMousePosition();
	Engine::Instance().m_render->painter().RenderTexture(*activeCursor.texture, mousePos + activeCursor.hitPoint, &activeCursor.rect, { activeCursor.scale, activeCursor.scale},0 );
}

void ModuleCursor::HideNormalCursor()
{
	SDL_ShowCursor(SDL_DISABLE);
}

void ModuleCursor::ShowNormalCursor()
{
	SDL_ShowCursor(SDL_ENABLE);
}

void ModuleCursor::HideCustomCursor()
{
	hiddenCustom = true;
}

void ModuleCursor::ShowCustomCursor()
{
	hiddenCustom = false;
}

void ModuleCursor::HideAllCursors()
{
	HideNormalCursor();
	HideCustomCursor();
}

void ModuleCursor::ShowAllCursors()
{
	ShowNormalCursor();
	ShowCustomCursor();
}

void ModuleCursor::SetCursor(SDL_Texture* _texture, SDL_Rect _rect, Vector2Int _hitPoint, float _scale)
{
 	activeCursor.texture = _texture;
	activeCursor.rect = _rect;
	activeCursor.hitPoint = _hitPoint;
	activeCursor.scale = _scale;

	if (activeCursor.texture != nullptr) {
		HideNormalCursor();
		ShowCustomCursor();
	}
	else {
		ShowNormalCursor();
		HideCustomCursor();
	}
}

void ModuleCursor::SetDefaultCursor(SDL_Texture* _texture, SDL_Rect _rect, Vector2Int _hitPoint, float _scale)
{
	defaultCursor.texture = _texture;
	defaultCursor.rect = _rect;
	defaultCursor.hitPoint = _hitPoint;
	defaultCursor.scale = _scale;

	if (activeCursor.texture == nullptr && defaultCursor.texture != nullptr && !hiddenCustom) {
		UseDefaultCursor();
	}
}

void ModuleCursor::UseDefaultCursor()
{
	SetCursor(defaultCursor.texture, defaultCursor.rect, defaultCursor.hitPoint, defaultCursor.scale);
}


