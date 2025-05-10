#include "UICanvas.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "UIElement.h"
#include "UIEmpty.h"


UICanvas::UICanvas()
{
	root = new UIEmpty({0,0}, {0,0});
	root->localdebug = true;
	Engine::Instance().m_render->AddToRenderQueue(*this);
}

UICanvas::~UICanvas()
{
	Engine::Instance().m_render->RemoveFromRenderQueue(*this);
	ClearCanvas();
	if (root != nullptr) {
		delete root;
		root = nullptr;
	}
}

void UICanvas::AddElementToCanvas(UIElement* element)
{
	if(!element->HasParent())
		element->SetParent(root);
	elements.emplace_back(element);
}

void UICanvas::RemoveElementFromCanvas(UIElement* element)
{
	if (element->HasParent() && element->GetParent() == root)
		element->SetParent(nullptr);
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
	root->ClearChilds();
	for (const auto& element : elements)
		delete element;

	elements.clear();
}

void UICanvas::SetPosition(Vector2 position)
{
	root->SetLocalPosition(position);
}

Vector2 UICanvas::GetPosition()
{
	return root->GetPosition();
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
	root->RenderElementDebug();
	for (const auto& element : elements)
		element->RenderElement();
	Engine::Instance().m_render->SetCameraMode(true);
}

void UICanvas::Render()
{
	
	RenderCanvas();
	
}