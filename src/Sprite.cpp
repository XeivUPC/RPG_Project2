#include "Sprite.h"

Sprite::Sprite(SDL_Texture* _texture, SDL_Rect _section, Vector2 _pivot, Vector2 _offset)
{
	texture = _texture;
	rect = _section;
	pivot = _pivot;
	offset = _offset;
}

SDL_Texture* Sprite::Texture() const
{
	return texture;
}

void Sprite::SetTexture(SDL_Texture* _texture)
{
	texture = _texture;
}

const SDL_Rect& Sprite::Section() const
{
	return rect;
}

const Vector2& Sprite::Pivot() const
{
	return pivot;
}

const Vector2& Sprite::Offset() const
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