#include "UIElement.h"
#include "Engine.h"
#include "ModuleInput.h"

UIElement::UIElement(Vector2Int _position, Vector2Int _size, Vector2 _pivot) : localPosition(_position), size(_size), pivot(_pivot)
{
	ForceUpdateTransforms();
}

UIElement::~UIElement()
{
	onMouseExit.Trigger();
}

void UIElement::UpdateElement()
{
	if (parent != nullptr) {
		scale = localScale*parent->scale;
		position = localPosition+parent->position;
		if (!parent->visible) {
			visible = false;
		}
		else
			visible = localVisible;

		if(parent->debug) {
			debug = true;
		}
		else
			debug = localdebug;
	}
	else {
		scale = localScale;
		position = localPosition;
		visible = localVisible;
	}

	if (!interactable) {
		if(isMouseOver)
			onMouseExit.Trigger();
		isMouseOver = false;
		return;
	}
	if (IsInBounds(Engine::Instance().m_input->GetMousePosition())) {
		if (!isMouseOver)
			onMouseEnter.Trigger();
		else
			onMouseOver.Trigger();
		isMouseOver = true;
	}
	else {
		if(isMouseOver)
			onMouseExit.Trigger();
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
	ForceUpdateTransforms();
}

void UIElement::SetLocalPosition(Vector2Int _localPosition)
{
	localPosition = _localPosition;
	ForceUpdateTransforms();
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
	if (parent!=nullptr)
		parent->childs.erase(remove(parent->childs.begin(), parent->childs.end(), this), parent->childs.end());

	parent = _parent;

	if (parent != nullptr) {
		parent->childs.emplace_back(this);
		ForceUpdateTransforms();
	}


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

	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->ForceUpdateTransforms();
	}
}

bool UIElement::IsInBounds(Vector2Int point)
{
	SDL_Rect bounds = GetBounds();
	if (point.x > bounds.x && point.x < bounds.x + bounds.w && point.y > bounds.y && point.y < bounds.y + bounds.h)
		return true;
	return false;
}

