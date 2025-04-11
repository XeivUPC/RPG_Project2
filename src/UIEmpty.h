#pragma once
#include "UIElement.h"

class UIEmpty : public UIElement {
public:
	UIEmpty(Vector2Int _position, Vector2Int _size, Vector2 _pivot = { 0,0 });
	~UIEmpty();

	// Inherited via UIElement
	void UpdateElement() override;
	// Inherited via UIElement
	void RenderElement() override;
	// Inherited via UIElement
	void RenderElementDebug() override;

public:

private:

private:

};