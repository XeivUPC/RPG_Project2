#pragma once
#include "Vector2.h" 
#include "Vector2Int.h" 
#include "SDL2/SDL.h"

struct Camera {
	Vector2 target{ 0,0 };
	Vector2Int viewport{ 0,0 };
	Vector2 offset = { 0,0 };
	float zoom = 1;

	SDL_Rect rect = {0,0,0,0};

	SDL_Rect GetRect() const {
		return { (int)target.x, (int)target.y, (int)(viewport.x * (1 / zoom)), (int)(viewport.y * (1 / zoom)) };
	}
};