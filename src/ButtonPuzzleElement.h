#pragma once
#include "PuzzleElement.h"
#include "Vector2Int.h"
#include "IInteractuable.h"
#include "IPooleable.h"

class PhysBody;
using namespace std;

class ButtonPuzzleElement : public PuzzleElement, public IInteractuable, public IPooleable {
public:

	ButtonPuzzleElement();
	~ButtonPuzzleElement();

	void Initialize(string _id, Vector2Int _position, bool value, vector<string> _targets);

	void RecieveCall(string _id, unordered_map<string, string> _params) override;
	void SendCall() override;
	void Complete() override;


	// Inherited via IInteractuable
	void Interact() override;

	// Heredado vía IPooleable
	void InitPoolObject() override;
	// Heredado vía IPooleable
	void ResetPoolObject() override;

	void SetPosition(Vector2 newPosition);

private:
	bool isPressed = false;
	vector<string> targets;

	PhysBody* body = nullptr;

};