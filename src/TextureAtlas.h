#pragma once
#include <unordered_map>
#include "Vector2Int.h"

using namespace std;

struct SDL_Texture;

class TextureAtlas {



public:
	struct AtlasSprite {
		Vector2Int origin;
		Vector2Int position;
		Vector2Int size;
	};

	inline TextureAtlas(SDL_Texture* _texture)
	{
		texture = _texture;
	}

	SDL_Texture* texture = nullptr;
	unordered_map<string, AtlasSprite> sprites;
};
