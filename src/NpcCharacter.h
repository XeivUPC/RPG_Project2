#pragma once
#include "Character.h"
#include "IPooleable.h"
#include "IInteractuable.h"

class NpcCharacter : public Character, public IPooleable, public IInteractuable {

public:

	NpcCharacter();
	~NpcCharacter();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;

	void SetNpcData(int _npcId, Vector2 _position);

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
	int npcId = 0;

protected:
	// Inherited via Character
	void Move() override;
protected:






};