#include "UICanvas.h"
#include "UIElement.h"

UICanvas::~UICanvas()
{
	for (const auto& element : elements)
		delete element;

	elements.clear();
}

void UICanvas::AddElementToCanvas(UIElement* element)
{
	elements.emplace_back(element);
}

void UICanvas::RemoveElementFromCanvas(UIElement* element)
{
	elements.erase(remove(elements.begin(), elements.end(), element), elements.end());
}

void UICanvas::UpdateCanvas()
{
	if (!visible)
		return;
	for (const auto& element : elements)
		element->UpdateElement();
}

void UICanvas::RenderCanvas()
{
	if (!visible)
		return;
	for (const auto& element : elements)
		element->RenderElement();
}
