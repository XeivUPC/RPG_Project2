#pragma once
#include "Character.h"
#include "IPooleable.h"
#include "IInteractuable.h"


#include <vector>

class Party;

class NpcCharacter : public Character, public IPooleable, public IInteractuable {

public:

	enum class MovementType { PingPong, Loop, StopAtEnd };

	NpcCharacter();
	~NpcCharacter();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;

	void SetNpcPath(vector<Vector2> _path, MovementType _movementType = MovementType::PingPong);

	bool SetCharacterId(int _charId) override;

	// Inherited via IInteractuable
	void Interact() override;

	// Heredado vía IPooleable
	void InitPoolObject() override;
	// Heredado vía IPooleable
	void ResetPoolObject() override;

public:

private:
	void SearchPath();

private:

	vector<Vector2> path;
	int pathPosition = 0;
	int pathDirection = 1;
	MovementType movementType = MovementType::PingPong;

protected:
	// Inherited via Character
	void Move() override;
	// Inherited via Character
	void Animate() override;
protected:

};