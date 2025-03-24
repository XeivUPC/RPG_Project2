#include "Vector2.h"
#include <vector>
#pragma once

class Path
{
	friend class ModulePathfinding;
private:
	Vector2 position;
	std::vector<Vector2Int> path;
	Vector2 target;


protected:
	
public:
	Path(Vector2& position);
};