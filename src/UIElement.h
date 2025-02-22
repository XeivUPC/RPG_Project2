#pragma once
#include "Vector2.h"
#include "ModuleRender.h"
#include <functional>
#include <string>
#include <vector>

using namespace std;

class UIElement {
public:
	UIElement(Vector2Int _position, Vector2Int _size, Vector2 _pivot = {0,0});
	virtual ~UIElement();

	virtual void UpdateElement();
	virtual void RenderElement();
	virtual void RenderElementDebug();

	const Vector2Int& GetPosition();
	const Vector2Int& GetConstLocalPosition();
	Vector2Int& GetLocalPosition();
	const float& GetScale();
	const float& GetConstLocalScale();
	float& GetLocalScale();

	virtual void SetLocalScale(float _localScale);
	virtual void SetLocalPosition(Vector2Int _localPosition);

	SDL_Rect GetBounds();

	/// Sets the UIElement from wich it will inherit position and scale
	void SetParent(UIElement* _parent);

	void ForceUpdateTransforms();

public:

	/// How To Use
	/// button->onMouseEvent.emplace_back([scope](parameters) {FunctionContent});
	vector<function<void()>> onMouseOver;
	vector<function<void()>> onMouseEnter;
	vector<function<void()>> onMouseExit;

	Vector2 pivot = {0,0};
	Vector2Int size = {100,50};

	bool debug = false;

private:
	
private:

protected:
	bool IsInBounds(Vector2Int point);
	void TriggerCallbacks(vector<function<void()>>& callbacks);

protected:
	float scale = 1;
	Vector2Int position = { 0,0 };

	/// If has a parent UIElement, this value will be added to the parent one
	Vector2Int localPosition = { 0,0 };
	/// If has a parent UIElement, this value will be added to the parent one
	float localScale = 1;

	UIElement* parent = nullptr;
	vector<UIElement*> childs;
	bool isMouseOver = false;

};