#pragma once
#include "TextureAtlas.h"
#include <string>

using namespace std;

struct SDL_Texture;

class AtlasFactory {

public:

	
	

	AtlasFactory();
	~AtlasFactory();

	TextureAtlas* CreateAtlas(SDL_Texture& texture, const string& xmlInfoPath) const;
	void DeleteAtlas(TextureAtlas& atlas) const;
};