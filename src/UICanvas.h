#pragma once
#include "IRendereable.h"
#include <vector>


using namespace std;

class UIElement;
class UIEmpty;

class UICanvas : public IRendereable {
public:
	virtual ~UICanvas() = 0;

	void AddElementToCanvas(UIElement* element);
	void RemoveElementFromCanvas(UIElement* element);

	void SendElementToFront(UIElement* element);
	void SendElementToBack(UIElement* element);
	void SendElementToPositionOrder(UIElement* element, int order);

	virtual void UpdateCanvas();
	void RenderCanvas();

	void SetInteractable(bool mode);

	bool IsInteractable();

	void ClearCanvas();

	void SetPosition(Vector2 position);
	Vector2 GetPosition();


public:

protected:
	UICanvas();
protected:
private:

private:
	bool interactable = true;
	vector<UIElement*> elements;
	UIEmpty* root = nullptr;

	// Inherited via IRendereable
	void Render() override;
};