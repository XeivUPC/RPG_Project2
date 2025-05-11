#include "UIEmpty.h"
#include "Engine.h"
#include "DrawingTools.h"

UIEmpty::UIEmpty(Vector2Int _position, Vector2Int _size, Vector2 _pivot) : UIElement(_position, _size, _pivot)
{
}

UIEmpty::~UIEmpty()
{
	for (size_t i = 0; i < childs.size(); i++)
	{
		delete childs[i];
	}
	childs.clear();
}

void UIEmpty::UpdateElement()
{
	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->UpdateElement();
	}

	UIElement::UpdateElement();
}

void UIEmpty::RenderElement()
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

void UIEmpty::RenderElementDebug()
{
	if(debug)
		Engine::Instance().m_render->painter().RenderBox(position, size, { scale,scale }, pivot, false, { 255,255,0,255 });
}
