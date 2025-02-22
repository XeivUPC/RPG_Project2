#include "Tilemap.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleAssetDatabase.h"
#include "DrawingTools.h"

Tilemap::Tilemap(SDL_Texture& _texture, Vector2Int _initialPosition, Vector2Int _tileSize, Vector2Int _tilemapSize, vector<int> _tiles, float _scale)
{
	SetTilemapData(_texture, move(_initialPosition), move(_tileSize), move(_tilemapSize), move(_tiles), _scale);
}

Tilemap::~Tilemap()
{
}

void Tilemap::SetTilemapData(SDL_Texture& _texture, Vector2Int _initialPosition, Vector2Int _tileSize, Vector2Int _tilemapSize, vector<int> _tiles, float _scale)
{
	texture = &_texture;
	initialPosition = _initialPosition;
	tileSize = _tileSize;
	tilemapSize = _tilemapSize;
	tiles = _tiles;
	scale = _scale;

	textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*texture);
}

void Tilemap::UpdateTilemap()
{
}

void Tilemap::RenderTilemap()
{
	const DrawingTools& painter = Engine::Instance().m_render->painter();
	Vector2Int drawingPos = { 0,0 };
	SDL_Rect rect = {0,0,tileSize.x,tileSize.y};
	for (int yPos = 0; yPos < tilemapSize.y; yPos++)
	{
		for (int xPos = 0; xPos < tilemapSize.x; xPos++)
		{
			drawingPos.x = xPos * tileSize.x;
			drawingPos.y = yPos * tileSize.y;

			int gid = GetTileRelativeIndex(xPos, yPos);

			GetTileRect(gid, rect);
			
			painter.RenderTexture(*texture, drawingPos+initialPosition, &rect, {scale,scale});
		}
	}
}

int Tilemap::GetTileRelativeIndex(int x, int y)
{
	return tiles[(y * tilemapSize.x) + x];
}
void Tilemap::GetTileRect(int gid, SDL_Rect& rect) {
	int relativeIndex = gid;
	rect.w = tileSize.x;
	rect.h = tileSize.y;
	rect.x = (tileSize.x) * (relativeIndex % tilemapSize.y);
	rect.y = (tileSize.y) * (relativeIndex / tilemapSize.y);
}