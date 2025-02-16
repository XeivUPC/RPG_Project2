#pragma once
#include "ModuleAssetDatabase.h"
#include <string>

class TextureFactory
{
	public:
		TextureFactory(SDL_Renderer& _renderer);
		~TextureFactory();

		SDL_Texture* CreateTexture(const string& path) const;
		void DeleteTexture(SDL_Texture& textureToDelete) const;
	public:

	private:
		SDL_Texture* const LoadSurface(SDL_Surface* surface) const;
	private:
		SDL_Renderer* renderer;
};