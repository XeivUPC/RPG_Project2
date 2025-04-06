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

	ClearCanvas();
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

void UICanvas::ClearCanvas()
{
	for (const auto& element : elements)
		delete element;

	elements.clear();
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
	Engine::Instance().m_render->SetCameraMode(false);
	for (const auto& element : elements)
		element->RenderElement();
	Engine::Instance().m_render->SetCameraMode(true);
}

void UICanvas::Render()
{
	
	RenderCanvas();
	
}