#pragma once
#include <vector>

using namespace std;

class UIElement;

class UICanvas {
public:
	virtual ~UICanvas() = 0;

	void AddElementToCanvas(UIElement* element);
	void RemoveElementFromCanvas(UIElement* element);

public:
	void UpdateCanvas();
	void RenderCanvas();

protected:
	UICanvas() = default;
protected:

private:

private:
	vector<UIElement*> elements;
};