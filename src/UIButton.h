#pragma once
#include "UIElement.h"

class UIImage;
class UIButton : public UIElement {
public:

	enum class ButtonStates{
		DEFAULT,
		HOVER,
		PRESSED,
		DISABLED
	};

	UIButton(SDL_Texture& _texture, Vector2Int _position, Vector2Int _size, SDL_Rect _defaultRect, Vector2 _pivot = {0,0});
	UIButton(Vector2Int _position, Vector2Int _size, SDL_Rect _defaultRect, Vector2 _pivot = { 0,0 }, SDL_Color _color = { 255,255,255,255 });
	~UIButton();

	// Inherited via UIElement
	void UpdateElement() override;
	// Inherited via UIElement
	void RenderElement() override;
	// Inherited via UIElement
	void RenderElementDebug() override;

	void AddRect(ButtonStates state, SDL_Rect rect);
	ButtonStates GetState();
	void SetState(ButtonStates _state);

public:
	SystemEvent<> onMouseDown;
	SystemEvent<> onMouseUp;
	SystemEvent<> onMouseClick;

	bool isEnabled = true;
private:
private:
	bool isMouseHolding = false;
	unordered_map<ButtonStates,SDL_Rect> rects;
	UIImage* image_Component;

	ButtonStates state = ButtonStates::DEFAULT;
};