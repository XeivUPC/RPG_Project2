#include "UIElement.h"
#include "Engine.h"
#include "ModuleInput.h"

UIElement::UIElement(Vector2Int _position, Vector2Int _size, Vector2 _pivot) : localPosition(_position), size(_size), pivot(_pivot)
{
}

UIElement::~UIElement()
{
}

void UIElement::UpdateElement()
{
	if (parent != nullptr) {
		scale = localScale*parent->scale;
		position = localPosition+parent->position;
		debug = parent->debug;
	}
	else {
		scale = localScale;
		position = localPosition;
	}

	if (IsInBounds(Engine::Instance().m_input->GetMousePosition())) {
		if (!isMouseOver)
			TriggerCallbacks(onMouseEnter);
		else
			TriggerCallbacks(onMouseOver);
		isMouseOver = true;
	}
	else {
		if(isMouseOver)
			TriggerCallbacks(onMouseExit);
		isMouseOver = false;
	}
}

void UIElement::RenderElement()
{
}

void UIElement::RenderElementDebug()
{
}


const Vector2Int& UIElement::GetPosition()
{

	return position;
}

const Vector2Int& UIElement::GetConstLocalPosition()
{
	return localPosition;
}

Vector2Int& UIElement::GetLocalPosition()
{
	return localPosition;
}

const float& UIElement::GetScale()
{
	return scale;
}

const float& UIElement::GetConstLocalScale()
{
	return localScale;
}

float& UIElement::GetLocalScale()
{
	return localScale;
}

void UIElement::SetLocalScale(float _localScale)
{
	localScale = _localScale;
}

void UIElement::SetLocalPosition(Vector2Int _localPosition)
{
	localPosition = _localPosition;
}


SDL_Rect UIElement::GetBounds()
{
	Vector2Int realSize = size * scale;
	Vector2Int realStartingCorner = { (int)(position.x - realSize.x * pivot.x),  (int)(position.y - realSize.y * pivot.y)};
	
	SDL_Rect rect = { realStartingCorner.x,realStartingCorner.y,realSize.x,realSize.y };
	return move(rect);
}

void UIElement::SetParent(UIElement* _parent)
{
	parent = _parent;
}

void UIElement::ForceUpdateTransforms()
{
	if (parent != nullptr) {
		scale = localScale * parent->scale;
		position = localPosition + parent->position;
	}
	else {
		scale = localScale;
		position = localPosition;
	}
}

bool UIElement::IsInBounds(Vector2Int point)
{
	SDL_Rect bounds = GetBounds();
	if (point.x > bounds.x && point.x < bounds.x + bounds.w && point.y > bounds.y && point.y < bounds.y + bounds.h)
		return true;
	return false;
}

void UIElement::TriggerCallbacks(vector<function<void()>>& callbacks)
{
	for (auto& callback : callbacks) {
		callback();
	}
}
