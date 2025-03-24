#pragma once
#include "Module.h"
#include "Vector2Int.h"
#include "box2d/box2d.h"
#include <vector>

#define MAX_ENEMIES 8;
enum AXIS
{
	AXIS_HORIZONTAL,
	AXIS_VERTICAL,
	AXIS_BOTH,
};

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();
	virtual bool Awake();
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

private:
	Vector2Int GetFirstTile(Vector2Int direction);
	Vector2Int DistanceToTile(Vector2Int dist, AXIS axis = AXIS_BOTH);
	Vector2Int TileToDistance(Vector2Int tile, AXIS axis = AXIS_BOTH);
};