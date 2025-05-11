#pragma once
#include "SimpleMapObject.h"
#include "CollisionSensor.h"

class PhysBody;

class SimpleTilemapChanger : public SimpleMapObject {
public:
	SimpleTilemapChanger();
	~SimpleTilemapChanger();

	bool PostUpdate() override;

	void SetExitPosition(Vector2 _exitPosition);
	void SetTargetTilemapPath(string _targetPath);
	void SetEntryPoint(int _entryPoint);
	void SetEntryTrigger(Vector2 _position, Vector2 size);

	void InitPoolObject() override;
	void ResetPoolObject() override;
public:

private:

private:

protected:
	Vector2 exitPosition = { 0,0 };
	int entryPoint = -1;

	string targetPath = "";
	CollisionSensor entrySensor;
	PhysBody* entryTrigger = nullptr;
};
