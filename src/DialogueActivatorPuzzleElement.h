#pragma once
#include "PuzzleElement.h"
#include "Vector2Int.h"
#include "IPooleable.h"
#include "CollisionSensor.h"

class PhysBody;
using namespace std;

class DialogueActivatorPuzzleElement : public PuzzleElement, public IPooleable {
public:

	DialogueActivatorPuzzleElement();
	~DialogueActivatorPuzzleElement();

	bool Update() override;
	void Render() override;

	void Initialize(string _id, Vector2Int _position, Vector2 _size, vector<string> _targets, bool _isEnabled, string _dialoguePath, int _dialogueState);

	void RecieveCall(string _id, unordered_map<string, string> _params) override;
	void SendCall() override;
	void Complete() override;

	// Heredado vía IPooleable
	void InitPoolObject() override;
	// Heredado vía IPooleable
	void ResetPoolObject() override;

	void SetPosition(Vector2 newPosition);

private:

	// Inherited via PuzzleElement
	bool Load() override;
	bool Save() override;

private:
	bool isBeingTriggered = false;
	bool isEnabled = false;

	vector<string> targets;

	PhysBody* body = nullptr;
	CollisionSensor sensor;

	string dialoguePath="";
	int dialogueState=0;

};