#pragma once
#include "Vector2.h"
#include "ModuleRender.h"
#include "SystemEvent.h"
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
	const float& GetAngle();
	const float& GetConstLocalAngle();
	float& GetLocalAngle();

	virtual void SetLocalScale(float _localScale);
	virtual void SetLocalPosition(Vector2Int _localPosition);
	virtual void SetLocalAngle(float _localAngle);

	SDL_Rect GetBounds();

	/// Sets the UIElement from wich it will inherit position and scale
	void SetParent(UIElement* _parent);
	bool HasParent() const;
	UIElement* GetParent() const;

	void ClearChilds();

	void ForceUpdateTransforms();

public:

	/// How To Use
	/// button->onMouseEvent.emplace_back([scope](parameters) {FunctionContent});
	SystemEvent<> onMouseOver;
	SystemEvent<> onMouseEnter;
	SystemEvent<> onMouseExit;

	Vector2 pivot = {0,0};
	Vector2Int size = {100,50};

	bool localdebug = false;
	bool interactable = true;
	bool localVisible = true;

private:
	
private:

protected:
	bool IsInBounds(Vector2Int point);

protected:
	Vector2Int position = { 0,0 };
	float scale = 1;
	float angle=0;

	/// If has a parent UIElement, this value will be added to the parent one
	Vector2Int localPosition = { 0,0 };
	/// If has a parent UIElement, this value will be added to the parent one
	float localScale = 1;
	/// If has a parent UIElement, this value will be added to the parent one
	float localAngle = 0;

	UIElement* parent = nullptr;
	vector<UIElement*> childs;
	bool isMouseOver = false;

	bool visible = true;
	bool debug = false;
};