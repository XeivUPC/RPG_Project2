#pragma once
#include "Module.h"
#include "IRendereable.h"
#include "Vector2Int.h"

#include <unordered_map>
#include <string>

using namespace std;

class ModuleDebug : public Module
{
	friend class Engine;
public:

	ModuleDebug(bool start_active = true);
	~ModuleDebug();

	// Inherited via IInitializable
	bool Start() override;
	// Inherited via IUpdateable
	bool Update() override;
	// Inherited via ICleanable
	bool CleanUp() override;

public:
	bool godmode = false;
};