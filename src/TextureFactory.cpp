#include "TextureFactory.h"
#include "Log.h"

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_surface.h"


TextureFactory::TextureFactory(SDL_Renderer& _renderer)
{
    renderer = &_renderer;

	LOG("Init Image library");

	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
	}

}

TextureFactory::~TextureFactory()
{
	IMG_Quit();
}

SDL_Texture* TextureFactory::CreateTexture(const string& path) const
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	SDL_Texture* texture = nullptr;
	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else {
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
		if (texture != nullptr)
		{
			return texture;
		}
		else {
			LOG("Unable to create texture from surface! SDL Error: %s", SDL_GetError());
		}
	}
	return nullptr;
}

void TextureFactory::DeleteTexture(SDL_Texture& textureToDelete) const
{
	SDL_DestroyTexture(&textureToDelete);
}

SDL_Texture* const TextureFactory::LoadSurface(SDL_Surface* surface) const
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s", SDL_GetError());
	}

	return texture;
}
