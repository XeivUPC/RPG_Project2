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

	
protected:
	// Inherited via Character
	void Move() override;
	// Inherited via Character
	void Animate() override;
protected:
	
};