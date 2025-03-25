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

	float speedModifier = 1;
	float runSpeedModifier=1.5f;
protected:
	// Inherited via Character
	void Move() override;
	// Inherited via Character
	void Animate() override;
protected:
	
};