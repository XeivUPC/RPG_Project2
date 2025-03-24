#pragma once
#include "Entity.h"

class PhysBody;
class Animator;

class Character : public Entity {
public:
	
	Character();
	~Character();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;

	void SetPosition(Vector2 newPosition) override;

public:

private:

private:	

protected:
	virtual void Animate() = 0;
	virtual void Move() = 0;
protected:
	string characterName = "NO_NAME";

	string textureId = "";
	SDL_Texture* texture = nullptr;
	Animator* animator;
	PhysBody* body = nullptr;

	Vector2 moveDirection = { 0,0 };
	Vector2 lastDirection = { 0,1 };
	float speed = 5;

	Vector2 previousPhysicsPosition = { 0,0 };
};