#pragma once
#include "Character.h"
#include "IPooleable.h"
#include "IInteractuable.h"


#include <vector>


class FollowerCharacter : public Character{

public:

	FollowerCharacter(Character* _characterToFollow, float _delayDistance, string _charId);
	~FollowerCharacter();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;

	void SetCharacterToFollow(Character* _characterToFollow);
	bool SetCharacterId(string _charId) override;

	float GetDelayDistance() const;

	void SetDelayDistance(float _delayDistance);

public:

private:
	void SearchPath();
private:
	int pathPosition = 0;

	Character* characterToFollow = nullptr;
	float delayDistance = 0;

	bool canBeRendered = false;

protected:
	// Inherited via Character
	void Move() override;
	// Inherited via Character
	void Animate() override;
protected:

};
