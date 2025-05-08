#include "UIAnimatedImage.h"
#include "Engine.h"
#include "DrawingTools.h"

UIAnimatedImage::UIAnimatedImage(Vector2Int _position, Vector2Int _size, Vector2 _pivot, SDL_Color _color) : UIElement(_position, _size, _pivot)
{

}

UIAnimatedImage::~UIAnimatedImage()
{

	for (size_t i = 0; i < childs.size(); i++)
	{
		delete childs[i];
	}
	childs.clear();

	animator.CleanUp();
}

void UIAnimatedImage::UpdateElement()
{
	animator.clip()->UpdateClip();

	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->UpdateElement();
	}

	UIElement::UpdateElement();
}

void UIAnimatedImage::RenderElement()
{
	if (!visible)
		return;

	if (animator.clip() == nullptr) {
		Engine::Instance().m_render->painter().RenderBox(position, size, { scale,scale }, pivot, true);
	}
	else {
		animator.clip()->Flip(flip);
		animator.clip()->RenderClip(position, scale);
	}

	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->RenderElement();
	}

	if (debug) {
		RenderElementDebug();
	}
}

void UIAnimatedImage::RenderElementDebug()
{
	Engine::Instance().m_render->painter().RenderBox(position, size, { scale,scale }, pivot, false, { 255,255,0,255 });
}

Animator* UIAnimatedImage::GetAnimator()
{
	return &animator;
}

JsonAnimator* UIAnimatedImage::GetJsonAnimator()
{
	return &animator;
}


