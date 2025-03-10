#pragma once
#include "SDL2/SDL.h"
#include "Vector2.h"
struct Sprite
{
	SDL_Texture* texture;
	SDL_Rect rect;
	Vector2 pivot;
	Vector2 offset;
};

