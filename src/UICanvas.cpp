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

void UICanvas::UpdateCanvas()
{
	for (const auto& element : elements)
		element->UpdateElement();
}

void UICanvas::RenderCanvas()
{
	for (const auto& element : elements)
		element->RenderElement();
}
