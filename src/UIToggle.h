#pragma once
#include "UIElement.h"

class UIButton;
class UIToggle : public UIElement {
public:
	UIToggle(SDL_Texture& _texture, Vector2Int _position, Vector2Int _size, SDL_Rect _activeRect, SDL_Rect _inactiveRect, Vector2 _pivot = { 0,0 }, bool _isOn = true);
	~UIToggle();
	// Inherited via UIElement
	void UpdateElement() override;
	// Inherited via UIElement
	void RenderElement() override;
	// Inherited via UIElement
	void RenderElementDebug() override;

	bool IsOn() const;
	void SetValue(bool _isOn, bool notify = true);

public:
	SystemEvent<bool> onValueChange;
private:

private:
	UIButton* button_component;
	bool isOn = false;

protected:

};
