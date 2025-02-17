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
	SetCursor(texture, rect);
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
	Engine::Instance().m_render->painter().RenderTexture(*texture, mousePos + hitPoint, &rect, {scale, scale},0 );
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
	texture = _texture;
	rect = _rect;
	hitPoint = _hitPoint;
	scale = _scale;

	if (texture != nullptr) {
		HideNormalCursor();
		ShowCustomCursor();
	}
	else {
		ShowNormalCursor();
		HideCustomCursor();
	}
}


