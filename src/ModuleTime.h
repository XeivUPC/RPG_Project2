#pragma once
#include "Module.h" 
#include <vector>
#include <unordered_map>


class ModuleTime : public Module {
	friend class Engine;
	public:
		ModuleTime(bool start_active = true);
		~ModuleTime();

	public:
		static double deltaTime;
		static float timeScale;
	private:
		// Inherited via IInitializable
		bool Start() override;
		// Inherited via ICleneable
		bool CleanUp() override;
		// Inherited via IUpdateable
		bool PreUpdate() override;

	private:
		Uint64 lastTime = 0;
		Uint64 currentTime = 0;
};

