#pragma once
#include "SDL2/SDL.h"
#include "Vector2.h"
struct Sprite
{
private:
	SDL_Texture* texture;
	SDL_Rect rect;
	Vector2 pivot;
	Vector2 offset;

public:
	Sprite(SDL_Texture* _texture, SDL_Rect _section, Vector2 _pivot = { 0,0 }, Vector2 _offset = { 0,0 });
	SDL_Texture*& Texture();
	SDL_Rect& Section();
	Vector2& Pivot();
	Vector2& Offset();
	void CleanUp();
	~Sprite();
};

