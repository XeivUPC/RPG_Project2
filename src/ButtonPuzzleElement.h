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

	void Render() override;

	void Initialize(string _id, Vector2Int _position, bool value, vector<string> _targets);

	void RecieveCall(string _id, unordered_map<string, string> _params) override;
	void SendCall() override;
	void Complete() override;


	// Inherited via IInteractuable
	void Interact(Vector2 from) override;

	// Heredado v�a IPooleable
	void InitPoolObject() override;
	// Heredado v�a IPooleable
	void ResetPoolObject() override;

	void SetPosition(Vector2 newPosition);
private:

	// Inherited via PuzzleElement
	bool Load() override;
	bool Save() override;

private:

	bool isPressed = false;
	vector<string> targets;
	SDL_Texture* texture = nullptr;

	PhysBody* body = nullptr;

};