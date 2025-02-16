#include "UIToggle.h"
#include "UIButton.h"

UIToggle::UIToggle(SDL_Texture& _texture, Vector2Int _position, Vector2Int _size, SDL_Rect _activeRect, SDL_Rect _inactiveRect, Vector2 _pivot, bool _isOn) : UIElement(_position, _size, _pivot)
{
	button_component = new UIButton(_texture, { 0,0 }, move(_size), move(_inactiveRect), move(_pivot));
	button_component->AddRect(UIButton::ButtonStates::PRESSED, _activeRect);
	button_component->SetParent(this);
	SetValue(_isOn);

	button_component->onMouseClick.emplace_back([this]() {SetValue(!isOn); });
}

UIToggle::~UIToggle()
{
	delete button_component;
}

void UIToggle::UpdateElement()
{
	UIElement::UpdateElement();
	button_component->UpdateElement();
	if (isOn)
	{
		button_component->SetState(UIButton::ButtonStates::PRESSED);
	}
	else {
		button_component->SetState(UIButton::ButtonStates::DEFAULT);
	}
}

void UIToggle::RenderElement()
{
	button_component->RenderElement();
}

void UIToggle::RenderElementDebug()
{
}

bool UIToggle::IsOn() const
{
	return isOn;
}

void UIToggle::SetValue(bool _isOn)
{
	isOn = _isOn;
	TriggerCallbacks(onValueChange, isOn);
}

void UIToggle::TriggerCallbacks(vector<function<void(bool)>>& callbacks, bool _value)
{
	for (auto& callback : callbacks) {
		callback(_value);
	}
}
