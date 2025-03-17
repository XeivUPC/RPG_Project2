#include "Sprite.h"

Sprite::Sprite(SDL_Texture* _texture, SDL_Rect _section, Vector2 _pivot, Vector2 _offset)
{
	texture = _texture;
	rect = _section;
	pivot = _pivot;
	offset = _offset;
}

Sprite::~Sprite()
{
	texture = nullptr;
}