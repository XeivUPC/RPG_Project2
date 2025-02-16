#include "FontFactory.h"
#include "LOG.h"

#include <SDL2/SDL_ttf.h>

FontFactory::FontFactory()
{
	int init = TTF_Init();

	if (init!=0)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", TTF_GetError());
	}
}

FontFactory::~FontFactory()
{
	TTF_Quit();
}

TTF_Font* FontFactory::CreateFont(const string& path, int ptSize) const
{
	TTF_Font* font = TTF_OpenFont(path.c_str(), ptSize);
	if (font == NULL)
	{
		LOG("Could not load font: %s. TTF_OpenFont: %s", path.c_str(), TTF_GetError());
	}
	else {
		return font;
	}
	return nullptr;
}

void FontFactory::DeleteFont(_TTF_Font& fontToDelete) const
{
	TTF_CloseFont(&fontToDelete);
}
