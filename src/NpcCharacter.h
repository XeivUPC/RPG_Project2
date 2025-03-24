#pragma once
#include "Character.h"
#include "IPooleable.h"
#include "IInteractuable.h"


#include <vector>


class NpcCharacter : public Character, public IPooleable, public IInteractuable {

public:

	enum class MovementType { PingPong, Loop, StopAtEnd };

	NpcCharacter();
	~NpcCharacter();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;

	void SetNpcId(int _npcId);
	void SetNpcPath(vector<Vector2> _path, MovementType _movementType = MovementType::PingPong);

	// Inherited via IInteractuable
	void Interact() override;

	// Heredado v�a IPooleable
	void InitPoolObject() override;
	// Heredado v�a IPooleable
	void ResetPoolObject() override;

public:

private:
	void SearchPath();
private:
	int npcId = 0;

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