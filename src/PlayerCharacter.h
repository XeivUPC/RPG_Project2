#pragma once
#include "Character.h"
#include "CollisionSensor.h"

class PlayerCharacter : public Character {

public:

	PlayerCharacter();
	~PlayerCharacter();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;

public:

private:
	void GetInput();
private:
	CollisionSensor interactionSensor;
	Vector2 lastDirection = { 0,1 };
protected:
	// Inherited via Character
	void Move() override;
protected:
	
};