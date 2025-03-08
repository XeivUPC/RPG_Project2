#pragma once
#include "UIElement.h"

class UIImage : public UIElement {
public:
	UIImage(SDL_Texture& _texture, Vector2Int _position, Vector2Int _size, Vector2 _pivot = {0,0}, bool _useRect = false, SDL_Rect _rect = { 0,0,0,0 }, SDL_Color _color = { 255,255,255,255 });
	UIImage(Vector2Int _position, Vector2Int _size, Vector2 _pivot = {0,0}, bool _useRect = false, SDL_Rect _rect = { 0,0,0,0 }, SDL_Color _color = { 255,255,255,255 });
	~UIImage();

	// Inherited via UIElement
	void UpdateElement() override;
	// Inherited via UIElement
	void RenderElement() override;
	// Inherited via UIElement
	void RenderElementDebug() override;

	void SetSprite(SDL_Texture& _sprite, bool _useRect = false, SDL_Rect _rect = {0,0,0,0});
	SDL_Texture& GetSprite();

	void SetColor(SDL_Color _color = { 255,255,255,255 });
	SDL_Color& GetColor();

public:

private:

private:
	SDL_Texture* sprite = nullptr;
	bool useRect;
	SDL_Rect rect = {0,0,0,0};
	SDL_Color color;
};