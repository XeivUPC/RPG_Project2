#pragma once
#include "Module.h"
#include "IRendereable.h"
#include "Vector2Int.h"

class ModuleCursor : public Module, public IRendereable
{
	friend class Engine;
public:

	ModuleCursor(bool start_active = true);
	~ModuleCursor();

	// Inherited via IInitializable
	bool Init() override;
	// Inherited via IInitializable
	bool Start() override;
	// Inherited via ICleanable
	bool CleanUp() override;
	// Inherited via IRendereable
	void Render() override;

	void HideNormalCursor();
	void ShowNormalCursor();
	void HideCustomCursor();
	void ShowCustomCursor();

	void HideAllCursors();
	void ShowAllCursors();
	
	void SetCursor(SDL_Texture* _texture, SDL_Rect _rect, Vector2Int _hitPoint = {0,0}, float _scale = 1);
	void SetDefaultCursor(SDL_Texture* _texture, SDL_Rect _rect, Vector2Int _hitPoint = { 0,0 }, float _scale = 1);
	void UseDefaultCursor();

public:

private:
	struct CursorData {
		SDL_Texture* texture = nullptr;
		SDL_Rect rect = { 0,0,0,0 };
		float scale = 1;

		Vector2Int hitPoint = { 0,0 };
	};
private:

	CursorData activeCursor;
	CursorData defaultCursor;

	bool hiddenAll = false;
	bool hiddenCustom = false;

};