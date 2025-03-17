#pragma once
#include "Vector2.h"

class ITransformable {
public:
	virtual void SetPosition(Vector2 newPosition) = 0;
	virtual void SetAngle(double newAngle) = 0;
	virtual void SetScale(float newScale) = 0;
	virtual void SetAnchor(Vector2 newAnchor) = 0;
	virtual Vector2 GetPosition() = 0;
	virtual double GetAngle() = 0;
	virtual float GetScale() = 0;
	virtual Vector2 GetAnchor() = 0;
	virtual ~ITransformable() {}
protected:
	Vector2 position = {0,0};
	Vector2 anchor = { 0.5f,0.5f };
	float scale = 1;
	double angle = 0;
};