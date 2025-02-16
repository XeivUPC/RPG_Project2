#pragma once
#include "Module.h" 


class ModuleScene : public Module {
	friend class Engine;
public:
	ModuleScene(bool start_active = true);
	virtual ~ModuleScene() = 0;

public:

private:
	// Inherited via IInitializable
	bool Init() override;
	// Inherited via IInitializable
	bool Start() override;
	// Inherited via IUpdateable
	bool PreUpdate() override;
	// Inherited via IUpdateable
	bool Update() override;
	// Inherited via IUpdateable
	bool PostUpdate() override;
	// Inherited via ICleanable
	bool CleanUp() override;

private:
	
};