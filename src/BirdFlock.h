#pragma once
#include "IPooleable.h"
#include "Entity.h"
#include "CollisionSensor.h"

#include <vector>

class Bird;
class PhysBody;

using namespace std;

class BirdFlock : public Entity, public IPooleable {
public:
	BirdFlock();
	~BirdFlock();

	bool Update() override;
	void Render() override;

	void SpawnBirds();
	void RemoveBird(Bird* birdToRemove);

	void Initialize(Vector2 position, float radius);

	// Inherited via IPooleable
	void InitPoolObject() override;
	void ResetPoolObject() override;

private:
	Vector2 GetRandomPointInside();

private:

	PhysBody* body = nullptr;
	CollisionSensor interactionSensor;

	Vector2 position;
	float radius;

	int minAmount = 1;
	int maxAmount = 5;

	int spawnPercentage = 60;
	bool hasBirds = false;

	vector<Bird*> birds;

};