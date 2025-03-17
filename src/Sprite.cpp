#include "Sprite.h"

Sprite::Sprite(SDL_Texture* _texture, SDL_Rect _section, Vector2 _pivot, Vector2 _offset)
{
	texture = _texture;
	rect = _section;
	pivot = _pivot;
	offset = _offset;
}

SDL_Texture*& Sprite::Texture()
{
	return texture;
}

SDL_Rect& Sprite::Section()
{
	return rect;
}

Vector2& Sprite::Pivot()
{
	return pivot;
}

Vector2& Sprite::Offset()
{
	return offset;
}

void Sprite::CleanUp()
{
	texture = nullptr;
}

Sprite::~Sprite()
{
	texture = nullptr;
}