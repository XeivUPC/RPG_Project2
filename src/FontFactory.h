#pragma once
#include "ModuleAssetDatabase.h"
#include <string>


struct _TTF_Font;

class FontFactory
{
public:
	FontFactory();
	~FontFactory();

	_TTF_Font* CreateFont(const string& path, int ptSize = 0) const;
	void DeleteFont(_TTF_Font& fontToDelete) const;

public:

private:

private:

};