#pragma once
#include "Vector2Int.h"
#include <vector>

using namespace std;

struct SDL_Texture;
struct SDL_Rect;

class Tilemap
{
public:
	Tilemap(SDL_Texture& _texture, Vector2Int _initialPosition, Vector2Int _tileSize, Vector2Int _tilemapSize, vector<int> _tiles, float _scale = 1);
	~Tilemap();

	void SetTilemapData(SDL_Texture& _texture, Vector2Int _initialPosition, Vector2Int _tileSize, Vector2Int _tilemapSize, vector<int> _tiles,float _scale = 1);

	void UpdateTilemap();
	void RenderTilemap();

private:
	int GetTileRelativeIndex(int x, int y);
	void GetTileRect(int gid, SDL_Rect& rect);
private:
	Vector2Int initialPosition = { 0,0 };

	Vector2Int tileSize;
	Vector2Int tilemapSize;

	Vector2Int textureSize;
	SDL_Texture* texture;

	vector<int> tiles;

	float scale = 1;
};
