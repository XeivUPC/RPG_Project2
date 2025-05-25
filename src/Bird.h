#pragma once
#include "IPooleable.h"
#include "Vector2.h"

class BirdFlock;
class Animator;

class Bird : public IPooleable {
public:
	Bird();
	~Bird();

	void UpdateBird();
	void RenderBird();

	void Scare(Vector2 positionFromUser);
	void Initialize(BirdFlock* _flock, Vector2 _position);

	// Inherited via IPooleable
	void InitPoolObject() override;
	void ResetPoolObject() override;

private:

	Animator* animator;

	bool scared = false;
	BirdFlock* flock = nullptr;

	Vector2 position;
	Vector2 moveDirection = { 0,0 };
	float speed = 132;
};