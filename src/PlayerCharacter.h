#pragma once
#include "Character.h"

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

protected:
	// Inherited via Character
	void Move() override;
protected:
	
};