#pragma once
#include "SDL2/SDL.h"
#include "Vector2.h"
#include "SystemEvent.h"
struct Sprite
{
private:
	SDL_Texture* texture;
	SDL_Rect rect;
	Vector2 pivot;
	Vector2 offset;

public:
	Sprite(SDL_Texture* _texture, SDL_Rect _section, Vector2 _pivot = { 0.5f,0.5f }, Vector2 _offset = { 0,0 });
	SDL_Texture* Texture() const;
	void SetTexture(SDL_Texture* _texture);
	const SDL_Rect& Section() const;
	const Vector2& Pivot() const;
	const Vector2& Offset() const;
	void CleanUp();
	~Sprite();

	SystemEvent<> onSpriteSelected;
};

