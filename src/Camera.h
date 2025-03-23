#pragma once
#include "Vector2.h" 
#include "Vector2Int.h" 
#include "SDL2/SDL.h"

struct Camera {
	Vector2 position{ 0,0 };
	Vector2Int viewport{ 0,0 };
	Vector2 offset = { 0,0 };
	float zoom = 1;

	SDL_Rect rect = {0,0,0,0};

	SDL_Rect GetRect() const {
		return { (int)(position.x + offset.x/zoom), (int)(position.y + offset.y/zoom), (int)(viewport.x * (1 / zoom)), (int)(viewport.y * (1 / zoom)) };
	}

	SDL_Rect GetNonOffsetRect() const {
		return { (int)(position.x ), (int)(position.y), (int)(viewport.x * (1 / zoom)), (int)(viewport.y * (1 / zoom)) };
	}
};