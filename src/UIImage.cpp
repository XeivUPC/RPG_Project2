#include "UIImage.h"
#include "Engine.h"
#include "DrawingTools.h"

UIImage::UIImage(SDL_Texture& _texture, Vector2Int _position, Vector2Int _size, Vector2 _pivot, bool _useRect, SDL_Rect _rect, SDL_Color _color) : UIElement(_position, _size, _pivot), sprite(&_texture), useRect(_useRect), rect(_rect), color(_color)
{
}

UIImage::~UIImage()
{
}

void UIImage::RenderElement()
{
	if (sprite == nullptr)
		Engine::Instance().m_render->painter().RenderBox(position, size, { scale,scale }, pivot, true, color);
	else
		Engine::Instance().m_render->painter().RenderTexture(*sprite, position, useRect ? &rect : NULL, { scale,scale }, 0, pivot,SDL_FLIP_NONE,color);

	if (debug)
		RenderElementDebug();
}

void UIImage::RenderElementDebug()
{
	if (sprite != nullptr)
		Engine::Instance().m_render->painter().RenderBox(position, {rect.w,rect.h}, { scale,scale }, pivot, false, { 0,0,255,255 });

	Engine::Instance().m_render->painter().RenderBox(position, size, { scale,scale }, pivot, false, { 255,255,0,255 });

	
}

void UIImage::SetSprite(SDL_Texture& _sprite, bool _useRect, SDL_Rect _rect)
{
	sprite = &_sprite;
	useRect = _useRect;
	rect = _rect;
}

SDL_Texture& UIImage::GetSprite()
{
	return *sprite;
}

void UIImage::SetColor(SDL_Color _color)
{
	color = _color;
}

SDL_Color& UIImage::GetColor()
{
	return color;
}
