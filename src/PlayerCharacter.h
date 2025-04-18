#pragma once
#include "Character.h"
#include "CollisionSensor.h"

class Party;

class PlayerCharacter : public Character {

public:

	PlayerCharacter();
	~PlayerCharacter();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;

	bool SetCharacterId(int _charId) override;

	//// Modify Party (active and full) Add/Remove/Get for Both *REMEMEBER TO CHANGE CharacterID, there is a function*
	//// Will be necessary to resize the followers array, in order to keep the correct amount. MAX: 3 followers Create functions inside the Character Class to controll this --> Add/Remove/GetFollowers.  The max followers amount can be controlled also by the Character Class



public:
	Party* party = nullptr;
private:
	void GetInput();
private:
	CollisionSensor interactionSensor;

	float distanceBetweenFollowers = 20;
	
protected:
	// Inherited via Character
	void Move() override;
	// Inherited via Character
	void Animate() override;
protected:
	
};