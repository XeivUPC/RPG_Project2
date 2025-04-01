#include "UIToggle.h"
#include "UIButton.h"

UIToggle::UIToggle(SDL_Texture& _texture, Vector2Int _position, Vector2Int _size, SDL_Rect _activeRect, SDL_Rect _inactiveRect, Vector2 _pivot, bool _isOn) : UIElement(_position, _size, _pivot)
{
	button_component = new UIButton(_texture, { 0,0 }, move(_size), move(_inactiveRect), move(_pivot));
	button_component->AddRect(UIButton::ButtonStates::PRESSED, _activeRect);
	button_component->SetParent(this);
	SetValue(_isOn);

	button_component->onMouseClick.Subscribe([this]() { if(interactable)SetValue(!isOn); });
}

UIToggle::~UIToggle()
{
	for (size_t i = 0; i < childs.size(); i++)
	{
		delete childs[i];
	}
	childs.clear();
}

void UIToggle::UpdateElement()
{

	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->UpdateElement();
	}

	UIElement::UpdateElement();

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
	if (!visible)
		return;
	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->RenderElement();
	}
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
	onValueChange.Trigger(isOn);
}

