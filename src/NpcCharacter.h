#pragma once
#include "Character.h"
#include "IPooleable.h"
#include "IInteractuable.h"
#include "IDialogable.h"


#include <vector>

class Party;

class NpcCharacter : public Character, public IPooleable, public IInteractuable, public IDialogable {

public:

	enum class MovementType { PingPong, Loop, StopAtEnd };

	NpcCharacter();
	~NpcCharacter();

	bool PreUpdate() override;
	bool Update() override;
	void Render() override;

	bool CleanUp() override;

	void SetNpcPath(vector<Vector2> _path, MovementType _movementType = MovementType::PingPong);
	void SetDialoguePath(string _dialoguePath);

	bool SetCharacterId(string _charId) override;

	// Inherited via IInteractuable
	void Interact(Vector2 from) override;

	// Inherited via IDialogable
	bool UseDialogue() override;

	// Heredado v�a IPooleable
	void InitPoolObject() override;
	// Heredado v�a IPooleable
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