#pragma once
#include "Entity.h"

class PhysBody;

class Character : public Entity {
public:
	
	Character();
	~Character();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;


public:

private:

private:	

protected:
	virtual void Move() = 0;;
protected:
	string characterName = "NO_NAME";

	string textureId = "";
	SDL_Texture* texture = nullptr;
	PhysBody* body = nullptr;

	Vector2 moveDirection = { 0,0 };
	float speed = 5;
};