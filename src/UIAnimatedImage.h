#pragma once
#include "UIElement.h"
#include "JsonAnimator.h"

class UIAnimatedImage : public UIElement {
public:
	UIAnimatedImage(Vector2Int _position, Vector2Int _size, Vector2 _pivot = { 0,0 }, SDL_Color _color = { 255,255,255,255 });
	~UIAnimatedImage();

	// Inherited via UIElement
	void UpdateElement() override;
	// Inherited via UIElement
	void RenderElement() override;
	// Inherited via UIElement
	void RenderElementDebug() override;

	Animator* GetAnimator();
	JsonAnimator* GetJsonAnimator();


public:
	bool flip = false;
private:

private:
	JsonAnimator animator;
};