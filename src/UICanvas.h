#pragma once
#include <vector>

using namespace std;

class UIElement;

class UICanvas {
public:
	virtual ~UICanvas() = 0;

	void AddElementToCanvas(UIElement* element);
	void RemoveElementFromCanvas(UIElement* element);

	void UpdateCanvas();
	void RenderCanvas();

	void SetInteractable(bool mode);

	bool IsInteractable();

public:

	bool visible = true;
protected:
	UICanvas() = default;
protected:

private:

private:
	bool interactable = true;
	vector<UIElement*> elements;
};