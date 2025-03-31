#include "UISlider.h"
#include "UIButton.h"
#include "UIImage.h"
#include "LOG.h"

#include "Engine.h"
#include "ModuleInput.h"

UISlider::UISlider(SDL_Texture& _bgTexture, Vector2Int _position, Vector2Int _size, SDL_Rect _bgRect, SDL_Texture& _thumbTexture, Vector2Int _thumbSize, SDL_Rect _defaultThumbRect, SDL_Rect _selectedThumbRect, Vector2 _pivot, float _value, float _minValue, float _maxValue, SliderMode _mode) : UIElement(_position, _size, _pivot), value(_value), minValue(_minValue), maxValue(_maxValue), mode(_mode)
{
	button_component = new UIButton(_thumbTexture, {0,0}, _thumbSize, _defaultThumbRect, { 0.5f,0.5f });
	button_component->SetParent(this);
	button_component->AddRect(UIButton::ButtonStates::PRESSED, _selectedThumbRect);
	button_component->onMouseDown.Subscribe([this]() {usingSlider = true; });
	button_component->onMouseUp.Subscribe([this]() {usingSlider = false; });

	image_component = new UIImage(_bgTexture, {0,0}, _size, _pivot, true, _bgRect);
	image_component->SetParent(this);

	FixValueLimits();
}

UISlider::~UISlider()
{
	for (size_t i = 0; i < childs.size(); i++)
	{
		delete childs[i];
	}
	childs.clear();
}

void UISlider::UpdateElement()
{
	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->UpdateElement();
	}

	UIElement::UpdateElement();


	switch (mode)
	{
	case UISlider::SliderMode::LeftToRight:
	case UISlider::SliderMode::RightToLeft:
		button_component->GetLocalPosition().y = (int)((size.y / 2.f) * scale - (size.y * scale * pivot.y));
		break;
	case UISlider::SliderMode::TopToDown:
	case UISlider::SliderMode::DownToTop:
		button_component->GetLocalPosition().x = (int)((size.x / 2.f) * scale - (size.x * scale * pivot.x));
		break;
	default:
		break;
	}

	if (usingSlider) {
		button_component->SetState(UIButton::ButtonStates::PRESSED);
		ModuleInput* input = Engine::Instance().m_input;
		Vector2Int mousePos = input->GetMousePosition();
		if (FixThumbBounds(mousePos)) {
			ForceUpdateTransforms();
			UpdateValue();
			onValueChange.Trigger(value);
		}
	}
	else
	{
		button_component->SetState(UIButton::ButtonStates::DEFAULT);
	}
}

void UISlider::RenderElement()
{
	if (!visible)
		return;
	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->RenderElement();
	}

	if (debug)
		RenderElementDebug();
}

void UISlider::RenderElementDebug()
{
}

void UISlider::SetLocalScale(float _scale)
{
	UIElement::SetLocalScale(_scale);
	UpdateThumbByValue();
}

void UISlider::SetValue(float _value)
{
	value = _value;
	FixValueLimits();
}

void UISlider::SetLimitValues(float _minValue, float _maxValue)
{
	minValue = _minValue;
	maxValue = _maxValue;
	FixValueLimits();
}

float UISlider::GetValue()
{
	return value;
}

float UISlider::GetMinValue()
{
	return minValue;
}

float UISlider::GetMaxValue()
{
	return maxValue;
}

bool UISlider::FixThumbBounds(Vector2Int thumPosition)
{
	Vector2Int mousePos = thumPosition;
	int halfThumbSize = 0;
	Vector2Int moveBounds;
	SDL_Rect bounds;
	Vector2Int pos;
	Vector2Int localPos;

	switch (mode)
	{
		case UISlider::SliderMode::LeftToRight:
		case UISlider::SliderMode::RightToLeft:
			halfThumbSize = (int)((button_component->size.x / 2) * scale);
			bounds = GetBounds();
			moveBounds = { bounds.x + halfThumbSize, bounds.x + (int)(size.x * scale) - halfThumbSize };

			if (mousePos.x < moveBounds.x)
				mousePos.x = moveBounds.x;
			else if (mousePos.x > moveBounds.y)
				mousePos.x = moveBounds.y;

			pos = button_component->GetPosition();
			if (pos.x != mousePos.x) {
				localPos = button_component->GetConstLocalPosition();
				button_component->SetLocalPosition({ localPos.x + mousePos.x - pos.x, localPos.y });
				return true;
			}
			return false;
			break;
		case UISlider::SliderMode::TopToDown:
		case UISlider::SliderMode::DownToTop:
			halfThumbSize = (int)((button_component->size.y / 2) * scale);
			bounds = GetBounds();
			moveBounds = { bounds.y + halfThumbSize, bounds.y + (int)(size.y * scale) - halfThumbSize };

			if (mousePos.y < moveBounds.x)
				mousePos.y = moveBounds.x;
			else if (mousePos.y > moveBounds.y)
				mousePos.y = moveBounds.y;

			pos = button_component->GetPosition();
			if (pos.y != mousePos.y) {
				localPos = button_component->GetConstLocalPosition();
				button_component->SetLocalPosition({ localPos.x,localPos.y + mousePos.y - pos.y });
				return true;
			}
			return false;
			break;
		default:
			break;
	}
	return false;
}

void UISlider::FixValueLimits()
{
	if (value < minValue)
		value = minValue;
	if (value > maxValue)
		value = maxValue;

	UpdateThumbByValue();
}

void UISlider::UpdateValue()
{
	ForceUpdateTransforms();
	button_component->ForceUpdateTransforms();

	SDL_Rect bounds = GetBounds();
	Vector2Int moveBounds;
	int halfThumbSize = 0;
	Vector2Int pos = button_component->GetPosition();

	switch (mode)
	{
		case UISlider::SliderMode::LeftToRight:
			halfThumbSize = (int)((button_component->size.x / 2) * scale);
			moveBounds = { bounds.x + halfThumbSize, bounds.x + (int)(size.x * scale) - halfThumbSize };
			value = minValue + (pos.x - moveBounds.x) * (maxValue - minValue) / (moveBounds.y - moveBounds.x);
			break;
		case UISlider::SliderMode::RightToLeft:
			halfThumbSize = (int)((button_component->size.x / 2) * scale);
			moveBounds = { bounds.x + halfThumbSize, bounds.x + (int)(size.x * scale) - halfThumbSize };
			value = minValue + (pos.x - moveBounds.x) * (maxValue - minValue) / (moveBounds.y - moveBounds.x);
			value = abs(value - maxValue);
			break;
		case UISlider::SliderMode::TopToDown:
			halfThumbSize = (int)((button_component->size.y / 2) * scale);
			moveBounds = { bounds.y + halfThumbSize, bounds.y + (int)(size.y * scale) - halfThumbSize };
			value = minValue + (pos.y - moveBounds.y) * (maxValue - minValue) / (moveBounds.x - moveBounds.y);
			value = abs(value - maxValue);
			break;
		case UISlider::SliderMode::DownToTop:
			halfThumbSize = (int)((button_component->size.y / 2) * scale);
			moveBounds = { bounds.y + halfThumbSize, bounds.y + (int)(size.y * scale) - halfThumbSize };
			value = minValue + (pos.y - moveBounds.y) * (maxValue - minValue) / (moveBounds.x - moveBounds.y);
			break;
		default:
			break;
	}
}

void UISlider::UpdateThumbByValue()
{
	ForceUpdateTransforms();
	button_component->ForceUpdateTransforms();

	SDL_Rect bounds = GetBounds();
	Vector2Int moveBounds;
	int halfThumbSize = 0;
	Vector2Int pos = button_component->GetPosition();
	float percentage;

	switch (mode)
	{
		case UISlider::SliderMode::LeftToRight:
			halfThumbSize = (int)((button_component->size.x / 2) * scale);
			moveBounds = { bounds.x + halfThumbSize, bounds.x + (int)(size.x * scale) - halfThumbSize };
			percentage = (value - minValue) / (maxValue - minValue);
			button_component->GetLocalPosition().x = (int)((moveBounds.y - moveBounds.x) * percentage - size.x * pivot.x * scale + halfThumbSize);
			break;
		case UISlider::SliderMode::RightToLeft:
			halfThumbSize = (int)((button_component->size.x / 2) * scale);
			moveBounds = { bounds.x + halfThumbSize, bounds.x + (int)(size.x * scale) - halfThumbSize };
			percentage = (value - minValue) / (maxValue - minValue);
			button_component->GetLocalPosition().x = (int)((moveBounds.y - moveBounds.x) * abs(percentage - 1) - size.x * pivot.x * scale + halfThumbSize);
			break;
		case UISlider::SliderMode::TopToDown:
			halfThumbSize = (int)((button_component->size.y / 2) * scale);
			moveBounds = { bounds.y + halfThumbSize, bounds.y + (int)(size.y * scale) - halfThumbSize };
			percentage = (value - minValue) / (maxValue - minValue);
			button_component->GetLocalPosition().y = (int)((moveBounds.y - moveBounds.x) * percentage - size.y * pivot.y * scale + halfThumbSize);
			break;
		case UISlider::SliderMode::DownToTop:
			halfThumbSize = (int)((button_component->size.y / 2) * scale);
			moveBounds = { bounds.y + halfThumbSize, bounds.y + (int)(size.y * scale) - halfThumbSize };
			percentage = (value - minValue) / (maxValue - minValue);
			button_component->GetLocalPosition().y = (int)((moveBounds.y - moveBounds.x) * abs(percentage - 1) - size.y * pivot.y * scale + halfThumbSize);
			break;
		default:
			break;
	}
}

