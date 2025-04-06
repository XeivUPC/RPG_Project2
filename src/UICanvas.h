#pragma once
#include "IRendereable.h"
#include <vector>


using namespace std;

class UIElement;

class UICanvas : public IRendereable {
public:
	virtual ~UICanvas() = 0;

	void AddElementToCanvas(UIElement* element);
	void RemoveElementFromCanvas(UIElement* element);

	void UpdateCanvas();
	void RenderCanvas();

	void SetInteractable(bool mode);

	bool IsInteractable();

	void ClearCanvas();

public:

protected:
	UICanvas();
protected:

private:

private:
	bool interactable = true;
	vector<UIElement*> elements;

	// Inherited via IRendereable
	void Render() override;
};