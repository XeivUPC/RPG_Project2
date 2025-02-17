#pragma once
#include "UIElement.h"
#include <string>

struct _TTF_Font;

class UITextBox : public UIElement {
public:

	enum class HorizontalAlignment {
		Left, Middle, Right
	};

	enum class VerticalAlignment {
		Top, Middle, Bottom
	};

	UITextBox(string _text, _TTF_Font& _font, int _fontSize, SDL_Color _color, Vector2Int _position, Vector2Int _size, Vector2 _pivot = { 0,0 }, HorizontalAlignment _hAlignment = HorizontalAlignment::Left, VerticalAlignment _vAlignment = VerticalAlignment::Top, bool _wrap = true);
	~UITextBox();

	// Inherited via UIElement
	void UpdateElement() override;
	// Inherited via UIElement
	void RenderElement() override;
	// Inherited via UIElement
	void RenderElementDebug() override;

	void ChangeFont(_TTF_Font& _font);
	_TTF_Font* GetFont();

public:
	int fontSize = 24;
	string text;
	bool wrap = true;
	SDL_Color color = {255,255,255,255};
	HorizontalAlignment horizontalAligment = HorizontalAlignment::Left;
	VerticalAlignment verticalAligment = VerticalAlignment::Top;
private:

private:
	_TTF_Font* font = nullptr;
};