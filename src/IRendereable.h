#pragma once
#include "Vector2Int.h"
class IRendereable {
public:
	virtual void Render() = 0; 
	virtual ~IRendereable() {}
	int renderLayer = 0;
	Vector2Int renderOffsetSorting = { 0,0 };

	bool isVisible = true;
};