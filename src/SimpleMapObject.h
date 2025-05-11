#pragma once
#include "Entity.h"
#include "IPooleable.h"
#include <vector>

class PhysBody;

class SimpleMapObject : public Entity, public IPooleable {
public:
	SimpleMapObject();
	~SimpleMapObject();
	void SetData(string _atlasId, string _textureId, Vector2Int _position, float _scale);
	void SetData(Vector2Int _position, float _scale);
	void AddBoxCollision(Vector2 _position, Vector2 size,bool bevel =true);
	void AddChainCollision(Vector2 _position, vector<Vector2> points);
	void AddCircleCollision(Vector2 _position,	float radiu);

	void Render() override;

	bool CleanUp() override;

	// Inherited via IPooleable
	void InitPoolObject() override;
	// Inherited via IPooleable
	void ResetPoolObject() override;

public:

private:

private:
	string textureId = "";
	SDL_Texture* texture = nullptr;
	SDL_Rect rect = { 0,0,0,0 };

	vector<PhysBody*> bodies;
};