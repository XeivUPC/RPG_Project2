#pragma once
#include "Character.h"
#include "CollisionSensor.h"


class Party;
class Inventory;
class CharacterSilhouette;

class PlayerCharacter : public Character {

public:

	PlayerCharacter();
	~PlayerCharacter();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;

	bool SetCharacterId(string _charId) override;

public:
	Party* party = nullptr;
	Inventory* inventory = nullptr;
private:
	void GetInput();
private:
	CollisionSensor interactionSensor;
	CharacterSilhouette* silhouette = nullptr;
	float distanceBetweenFollowers = 20;
	
protected:
	// Inherited via Character
	void Move() override;
	// Inherited via Character
	void Animate() override;
protected:
	
};