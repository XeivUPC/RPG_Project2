#include "ModuleCursor.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "DrawingTools.h"
#include "LOG.h"

ModuleCursor::ModuleCursor(bool start_active) : Module(start_active)
{

}

ModuleCursor::~ModuleCursor()
{
	cursors.clear();
}

bool ModuleCursor::Init()
{
	renderLayer = 10000;
	return true;
}

bool ModuleCursor::Start()
{
	Engine::Instance().m_render->AddToRenderQueue(*this);
	SetCursor(activeCursor);
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

	Engine::Instance().m_render->painter().RenderTexture(*activeCursor.texture, mousePos + activeCursor.hitPoint* activeCursor.scale, &activeCursor.rect, {activeCursor.scale, activeCursor.scale}, 0);
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

void ModuleCursor::AddCursor(string id, SDL_Texture* _texture, SDL_Rect _rect, Vector2Int _hitPoint, float _scale)
{
	CursorData data = CreateCursor(_texture, _rect, _hitPoint, _scale);
	cursors.emplace(id, data);
}

void ModuleCursor::AddDefaultCursor(SDL_Texture* _texture, SDL_Rect _rect, Vector2Int _hitPoint, float _scale)
{
	defaultCursor.texture = _texture;
	defaultCursor.rect = _rect;
	defaultCursor.hitPoint = _hitPoint;
	defaultCursor.scale = _scale;

	if (activeCursor.texture == nullptr && defaultCursor.texture != nullptr && !hiddenCustom) {
		SelectDefaultCursor();
	}
}

ModuleCursor::CursorData ModuleCursor::CreateCursor(SDL_Texture* _texture, SDL_Rect _rect, Vector2Int _hitPoint, float _scale)
{
	CursorData newCursor{ _texture, _rect, _hitPoint, _scale };
	return newCursor;
}

void ModuleCursor::SetCursor(const CursorData& data)
{
	activeCursor.texture = data.texture;
	activeCursor.rect = data.rect;
	activeCursor.hitPoint = data.hitPoint;
	activeCursor.scale = data.scale;

	if (activeCursor.texture != nullptr) {
		HideNormalCursor();
		ShowCustomCursor();
	}
	else {
		ShowNormalCursor();
		HideCustomCursor();
	}
}

void ModuleCursor::SelectCursor(string id)
{
	if (cursors.count(id) != 0)
		SetCursor(cursors[id]);
	else
		LOG("No Cursor founded");
}

void ModuleCursor::SelectDefaultCursor()
{
	SetCursor(defaultCursor);
}


