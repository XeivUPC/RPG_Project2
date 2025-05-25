#pragma once
#include "PuzzleElement.h"
#include "Vector2Int.h"
#include "IPooleable.h"
#include "CollisionSensor.h"

class PhysBody;
using namespace std;

class TriggerPuzzleElement : public PuzzleElement, public IPooleable {
public:

	TriggerPuzzleElement();
	~TriggerPuzzleElement();

	bool Update() override;
	void Render() override;

	void Initialize(string _id, Vector2Int _position, float _size, vector<string> _targets, bool callOnEnter=true, bool callOnExit=true);

	void RecieveCall(string _id, unordered_map<string, string> _params) override;
	void SendCall() override;
	void Complete() override;

	// Heredado vía IPooleable
	void InitPoolObject() override;
	// Heredado vía IPooleable
	void ResetPoolObject() override;

	void SetPosition(Vector2 newPosition);

private:
	bool isBeingTriggered = false;
	bool onEnter = false;
	bool onExit = false;

	vector<string> targets;
	SDL_Texture* texture = nullptr;

	PhysBody* body = nullptr;
	CollisionSensor sensor;

};