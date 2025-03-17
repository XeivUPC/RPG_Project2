#include "UICanvas.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "UIElement.h"


UICanvas::UICanvas()
{
	Engine::Instance().m_render->AddToRenderQueue(*this);
}

UICanvas::~UICanvas()
{
	Engine::Instance().m_render->RemoveFomRenderQueue(*this);

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

void UICanvas::SetInteractable(bool mode)
{
	interactable = mode;
	for (const auto& element : elements)
		element->interactable = mode;
}

bool UICanvas::IsInteractable()
{
	return interactable;
}

void UICanvas::UpdateCanvas()
{
	if (!isVisible)
		return;
	for (const auto& element : elements)
		element->UpdateElement();
}

void UICanvas::RenderCanvas()
{
	for (const auto& element : elements)
		element->RenderElement();
}

void UICanvas::Render()
{
	RenderCanvas();
}