#pragma once
#include "IRendereable.h"

class Character;

class CharacterSilhouette : public IRendereable {
public:
	CharacterSilhouette();
	~CharacterSilhouette();

	void SetCharacter(Character* _character);

	// Inherited via IRendereable
	void Render() override;

public:
	int opacity = 30;

private:
	Character* character = nullptr;


	

};
