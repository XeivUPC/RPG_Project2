#pragma once
#include "Entity.h"

class CameraController : public Entity {
public:

	CameraController();
	~CameraController();

	bool PostUpdate() override;

	bool CleanUp() override;

	void SetTarget(Entity* entity);
	void SetOffset(Vector2 offset);


public:

private:
	void Move();
private:
	Entity* target = nullptr;
	float deadZoneHorizontalRadius=60;
	float deadZoneVerticalRadius=30;

	float smoothSpeed = 5.0f;
	Vector2 lastTargetPosition;         
	float stationaryTimer = 0.0f;  
	float stationaryThreshold = 1.0f;  
	float recenterSpeed = 3.0f;
};