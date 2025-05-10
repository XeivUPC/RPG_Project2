#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "Vector2.h"
#include "IPooleable.h"
#include "Entity.h"
#include "CollisionSensor.h"

using namespace std;

class Item;
class PhysBody;

class OverworldItem : public Entity, public IPooleable
{
private:
	Item* itemRef = nullptr;
	int quantity = 0;
	SDL_Texture* texture = nullptr;
	SDL_Rect rectangle = { 0,0,0,0 };
	void Reset();


	PhysBody* body = nullptr;
	CollisionSensor interactionSensor;


public:
	OverworldItem();
	~OverworldItem();
	void Initialize(Item* item, int q, Vector2 position);
	void PickUp();
	string GetId() const;
	int GetAmount() const;
	const Item* GetItemRef() const;

	// Heredado vía IUpdateable
	bool Update() override;

	// Heredado vía IRendereable
	void Render() override;

	// Heredado vía IPooleable
	void InitPoolObject() override;
	void ResetPoolObject() override;


};