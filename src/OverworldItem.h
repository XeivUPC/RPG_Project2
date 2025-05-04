#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "Vector2.h"
#include "IPooleable.h"
using namespace std;

class Item;

class OverworldItem : public IPooleable
{
private:
	string id = "";
	int quantity = 0;
	SDL_Texture* texture = nullptr;
	SDL_Rect rectangle = { 0,0,0,0 };
	void Reset();
public:
	OverworldItem();
	~OverworldItem();
	void Initialize(Item item, int q, Vector2 position);
	void PickUp();

	// Heredado vía IPooleable
	void InitPoolObject() override;
	void ResetPoolObject() override;
};