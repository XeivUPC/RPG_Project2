#include "UIButton.h"
#include "UIImage.h"
#include "Engine.h"
#include "ModuleInput.h"

UIButton::UIButton(SDL_Texture& _texture, Vector2Int _position, Vector2Int _size, SDL_Rect _defaultRect, Vector2 _pivot) : UIElement(_position, _size, _pivot)
{
	AddRect(ButtonStates::DEFAULT,  move(_defaultRect));
	image_Component = new UIImage(_texture, {0,0}, move(_size), move(_pivot), true, move(_defaultRect));
	image_Component->SetParent(this);
}

UIButton::UIButton(Vector2Int _position, Vector2Int _size, SDL_Rect _defaultRect, Vector2 _pivot, SDL_Color _color) : UIElement(_position, _size, _pivot)
{
	AddRect(ButtonStates::DEFAULT, move(_defaultRect));
	image_Component = new UIImage({ 0,0 }, move(_size), move(_pivot), true, move(_defaultRect), move(_color));
	image_Component->SetParent(this);
}

UIButton::~UIButton()
{
	for (size_t i = 0; i < childs.size(); i++)
	{
		delete childs[i];
	}
	childs.clear();
	rects.clear();
}

void UIButton::UpdateElement()
{

	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->UpdateElement();
	}

	if (!isEnabled) {
		isMouseHolding = false;
		SetState(ButtonStates::DISABLED);
		return;
	}

	UIElement::UpdateElement();

	ModuleInput* input = Engine::Instance().m_input;
	KeyState mouseClickState = input->GetMouseButtonDown(SDL_BUTTON_LEFT);
	
	if (isMouseOver) {
		if (mouseClickState == KEY_DOWN) {
			onMouseDown.Trigger();
			SetState(ButtonStates::PRESSED);
			isMouseHolding = true;
		}
		else if (mouseClickState == KEY_REPEAT) {
			//isMouseHolding = true;
			SetState(ButtonStates::PRESSED);
		}
		else if (mouseClickState == KEY_UP) {
			onMouseUp.Trigger();
			if (isMouseHolding) {
				onMouseClick.Trigger();
			}
			isMouseHolding = false;
		}
		else if(mouseClickState == KEY_IDLE){
			SetState(ButtonStates::HOVER);
		}
	}
	else{
		if ((mouseClickState == KEY_UP || !interactable) && isMouseHolding) {
			onMouseUp.Trigger();
			isMouseHolding = false;
		}
		SetState(ButtonStates::DEFAULT);
	}

}

void UIButton::RenderElement()
{
	if (!visible)
		return;
	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->RenderElement();
	}
	if (debug) {
		RenderElementDebug();
	}
}

void UIButton::RenderElementDebug()
{

}

void UIButton::AddRect(ButtonStates state, SDL_Rect rect)
{
	rects[state] = rect;
}

UIButton::ButtonStates UIButton::GetState()
{
	return state;
}

void UIButton::SetState(ButtonStates _state)
{
	if (state == _state)
		return;
	state = _state;

	if (rects.count(state) > 0) {
		image_Component->SetSprite(image_Component->GetSprite(), true, rects[state]);
	}
	else
	{
		image_Component->SetSprite(image_Component->GetSprite(), true, rects[ButtonStates::DEFAULT]);
	}
}
