#pragma once
#include "UIElement.h"
#include "UITextBox.h"
#include "ChronoTimer.h"
#include <string>

struct _TTF_Font;
class UIImage;

class UIInputField : public UIElement {
public:

	UIInputField(string _defaultText, string _text, _TTF_Font& _font, int _fontSize, SDL_Color _color, SDL_Color _defaultColor, Vector2Int _position, Vector2Int _size, Vector2 _pivot = { 0,0 }, UITextBox::HorizontalAlignment _hAlignment = UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment _vAlignment = UITextBox::VerticalAlignment::Top, bool _wrap = true);
	~UIInputField();

	// Inherited via UIElement
	void UpdateElement() override;
	// Inherited via UIElement
	void RenderElement() override;
	// Inherited via UIElement
	void RenderElementDebug() override;



public:
	UITextBox* textComponent;
private:

private:
	//UIImage* imageComponent;

	bool isSelected = false;
	int writerPointer = -1;

	ChronoTimer backspaceTimer;
	int backspaceRemovingSpeedMS = 50;
	int backspaceSaveRemovingSpeedMS = 500;
	int backspaceCount = 0;

	ChronoTimer movingTimer;
	int movingSpeedMS = 50;
	int movingSaveSpeedMS = 500;
	int movingCount = 0;

	string defaultText = "Enter text...";
	SDL_Color defaultColor;
};