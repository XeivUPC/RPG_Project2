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

	const string& GetText();
	string& GetModificableText();
	void SetText(string _text);

	const int& GetFontSize();
	void SetFontSize(int _fontSize);

	const bool& GetIfWrap();
	void SetWrap(bool _wrap);

	const SDL_Color& GetColor();
	void SetColor(SDL_Color _color);

	_TTF_Font& GetFont();
	void SetFont(_TTF_Font& _font);

	const HorizontalAlignment& GetHorizontalAligment();
	void SetHorizontalAligment(HorizontalAlignment _horizontalAligment);

	const VerticalAlignment& GetVerticalAligment();
	void SetVerticalAligment(VerticalAlignment _verticalAligment);

	void SetDirty();
	bool GetIfDirty();

	Vector2 ProccesTextSize();
	Vector2 ProccesTextSize(string& _text);

public:
	
private:

private:
	_TTF_Font* font = nullptr;
	int fontSize = 24;
	string text;
	bool wrap = true;
	SDL_Color color = { 255,255,255,255 };
	HorizontalAlignment horizontalAligment = HorizontalAlignment::Left;
	VerticalAlignment verticalAligment = VerticalAlignment::Top;

	bool dirty = true;
	SDL_Texture* texture = nullptr;


};