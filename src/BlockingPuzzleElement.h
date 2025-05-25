#pragma once

#include "PuzzleElement.h"
#include "Vector2Int.h"
#include "IInteractuable.h"
#include "IPooleable.h"

class PhysBody;
using namespace std;

class BlockingPuzzleElement : public PuzzleElement, public IPooleable {
public:

	BlockingPuzzleElement();
	~BlockingPuzzleElement();

	void Render() override;

	void Initialize(string _id, Vector2Int _position, Vector2 _size , bool blocks);

	void RecieveCall(string _id, unordered_map<string, string> _params) override;
	void SendCall() override;
	void Complete() override;


	// Heredado vía IPooleable
	void InitPoolObject() override;
	// Heredado vía IPooleable
	void ResetPoolObject() override;

	void SetPosition(Vector2 newPosition);
private:

	void Enable();
	void Disable();

private:
	bool isBlocking = false;
	PhysBody* body = nullptr;

	Vector2 size = { 0,0 };
};