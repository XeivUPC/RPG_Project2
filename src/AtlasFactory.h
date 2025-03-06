#pragma once
#include <unordered_map>
#include "Vector2Int.h"
#include <string>

using namespace std;

struct SDL_Texture;


class Atlas {

	

public:
	struct AtlasSprite {
		Vector2Int origin;
		Vector2Int position;
		Vector2Int size;
	};

	inline Atlas(SDL_Texture* _texture)
	{
		texture = _texture;
	}

	SDL_Texture* texture = nullptr;
	unordered_map<string, AtlasSprite> sprites;
};


class AtlasFactory {

public:

	
	

	AtlasFactory();
	~AtlasFactory();

	Atlas* CreateAtlas(SDL_Texture& texture, const string& xmlInfoPath) const;
	void DeleteAtlas(Atlas& atlas) const;
};